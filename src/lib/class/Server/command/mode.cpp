/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:04:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "channel_modes.hpp"
#include "class/Exceptions.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include "user_modes.hpp"
#include <list>
#include <sstream>

enum
{
	Set,
	Cleared
};

struct ParsingTools
{
	std::vector<std::string>::const_iterator       current_word;
	std::vector<std::string>::const_iterator       next_word;
	std::vector<std::string>::const_iterator const end_of_words;
	std::string::const_iterator                    current_character;

	// Constructors
	ParsingTools(
		std::vector<std::string>::const_iterator const &current_word,
		std::vector<std::string>::const_iterator const &end_of_words) :
		current_word(current_word),
		next_word(current_word + 1),
		end_of_words(end_of_words),
		current_character(current_word->begin())
	{
	}

	// Destructor
	~ParsingTools(){};
};

//---------------------------------------------------------------------------------------------------------------------+
//                                                      USER MODE                                                      |
//---------------------------------------------------------------------------------------------------------------------+

/**
 * @brief Sends an ERR_USERSDONTMATCH reply to a client.
 *
 * @param client The client to send the reply to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_users_dont_match(Client &client)
{
	std::string const msg = format_reply(ERR_USERSDONTMATCH);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Sends a RPL_UMODEIS reply to a client.
 *
 * @param client The client to send the reply to.
 * @param nickname The nickname of the user for which modes are to be sent (should be the client's nickname).
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void reply_user_mode_is(Client &client, NickName const &nickname)
{
	std::string const modes_as_string = client.get_modes().to_string();
	std::string const msg = format_reply(RPL_UMODEIS, &nickname, &modes_as_string);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Adds a user mode to the list of user modes to be set + Removes it from the list of user modes to be cleared.
 *
 * @param modes The current modes of the user for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param identifier The identifier of the user mode to set and clear.
 *
 * @return
 * `true` if the user mode was successfully set and cleared,
 * `false` otherwise (means that the user mode is unknown).
 */
inline static bool save_user_mode_to_be_set(
	Client::Modes const &modes,
	Client::Modes *const modes_to_be,
	char const           identifer)
{
	UserModeIterator const cit = user_modes.find(identifer);

	if (cit == user_modes.end())
		return false;

	UserMode const &mode = cit->second;

	if (mode != LocalOperator && mode != Away)
	{
		if (!modes.is_set(mode))
			modes_to_be[Set].set(mode);
		modes_to_be[Cleared].clear(mode);
	}
	return true;
}

/**
 * @brief Adds a user mode to the list of user modes to be cleared + Removes it from the list of user modes to be set.
 *
 * @param modes The current modes of the user for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param identifier The identifier of the user mode to set and clear.
 *
 * @return
 * `true` if the user mode was successfully set and cleared,
 * `false` otherwise (means that the user mode is unknown).
 */
inline static bool save_user_mode_to_be_cleared(
	Client::Modes const &modes,
	Client::Modes *const modes_to_be,
	char const           identifer)
{
	UserModeIterator const cit = user_modes.find(identifer);

	if (cit == user_modes.end())
		return false;

	UserMode const &mode = cit->second;

	if (mode != Away)
	{
		if (modes.is_set(mode))
			modes_to_be[Cleared].set(mode);
		if (mode != LocalOperator)
			modes_to_be[Set].clear(mode);
	}
	return true;
}

/**
 * @brief Saves the changes to be made to the modes of a user in two lists of modes.
 *
 * @param modes The current modes of the user for which the changes are to be made.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param changes The changes to be made to the user's modes.
 *
 * @return `true` if there is an unknown mode change, `false` otherwise.
 */
inline static bool save_changes_for_user(
	Client::Modes const &modes,
	Client::Modes *const modes_to_be,
	std::string const   &changes)
{
	for (std::string::const_iterator current_character = changes.begin(); current_character < changes.end();)
		switch (*current_character)
		{
		case '+':
			do ++current_character;
			while (save_user_mode_to_be_set(modes, modes_to_be, *current_character));
			break;
		case '-':
			do ++current_character;
			while (save_user_mode_to_be_cleared(modes, modes_to_be, *current_character));
			break;
		default:
			return true;
		}
	return false;
}

/**
 * @brief Sends an ERR_UMODEUNKNOWNFLAG reply to a client.
 *
 * @param client The client to send the reply to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_user_mode_unknown_flag(Client &client)
{
	std::string const msg = format_reply(ERR_UMODEUNKNOWNFLAG);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Applies changes to the modes of a user.
 *
 * @param client The Client instance to apply the changes to.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_changes_for_user(Client &client, Client::Modes *const modes_to_be)
{
	if (client.has_mode(Bot))
	{
		if (modes_to_be[Cleared].is_set(Bot))
			client.clear_mode(Bot);
	}
	else
	{
		if (modes_to_be[Set].is_set(Bot))
			client.set_mode(Bot);
	}
	if (client.has_mode(LocalOperator))
	{
		if (modes_to_be[Cleared].is_set(LocalOperator))
			client.clear_mode(LocalOperator);
	}
	else
	{
		if (modes_to_be[Set].is_set(LocalOperator))
			client.set_mode(LocalOperator);
	}
	if (client.has_mode(Away))
	{
		if (modes_to_be[Cleared].is_set(Away))
			client.clear_mode(Away);
	}
	else
	{
		if (modes_to_be[Set].is_set(Away))
			client.set_mode(Away);
	}
	if (client.has_mode(Invisible))
	{
		if (modes_to_be[Cleared].is_set(Invisible))
			client.clear_mode(Invisible);
	}
	else
	{
		if (modes_to_be[Set].is_set(Invisible))
			client.set_mode(Invisible);
	}
	if (client.has_mode(WallopsListener))
	{
		if (modes_to_be[Cleared].is_set(WallopsListener))
			client.clear_mode(WallopsListener);
	}
	else
	{
		if (modes_to_be[Set].is_set(WallopsListener))
			client.set_mode(WallopsListener);
	}
}

/**
 * @brief Gets the current modes of a user, or applies some changes to the mode of a user.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void mode_for_user(Client &sender, std::vector<std::string> const &parameters)
{
	NickName const &nickname = parameters[0];

	if (nickname != sender.get_nickname())
		return error_users_dont_match(sender);

	if (parameters.size() == 1)
		return reply_user_mode_is(sender, nickname);

	Client::Modes modes_to_be[2];

	if (save_changes_for_user(sender.get_modes(), modes_to_be, parameters[1]))
		return error_user_mode_unknown_flag(sender);
	apply_changes_for_user(sender, modes_to_be);

	std::string const msg =
		"MODE " + sender.get_nickname() + " +" + modes_to_be[Set].to_string() + "-" + modes_to_be[Cleared].to_string();

	sender.append_to_msg_out(msg);
}

//--------------------------------------------------------------------------------------------------------------------+
//                                                    CHANNEL MODE                                                    |
//--------------------------------------------------------------------------------------------------------------------+

/**
 * @brief Sends an ERR_NOCHANMODES reply to a client.
 *
 * @param client The client to send the reply to.
 * @param name The name of the channel for which modes are not supported.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_no_channel_modes(Client &client, ChannelName const &name)
{
	std::string const msg = format_reply(ERR_NOCHANMODES, &name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief
 * Fills a list with replies, each containing the same channel name, and a user mask.
 * The list is then ended with a specific reply to indicate the end of the list.
 *
 * @param list The list to fill with replies.
 * @param masks The user masks to format the replies with.
 * @param rpl_list The reply code to use for the list.
 * @param rpl_end_of_list The reply code to use for the end of the list.
 * @param name The name of the channel for which the list is to be created.
 *
 * @return `true` if an error occurred while formatting a reply, `false` otherwise.
 */
inline static bool fill_list_of_replies(
	std::list<std::string>   &list,
	std::set<NickName> const &masks,
	int                       rpl_list,
	int                       rpl_end_of_list,
	ChannelName const        &name)
{
	if (!masks.empty())
	{
		for (std::set<NickName>::const_iterator cit = masks.begin(); cit != masks.end(); ++cit)
		{
			std::string const msg = format_reply(rpl_list, &name, &*cit);

			if (msg.empty())
				return true;

			list.push_back(msg);
		}

		std::string const msg = format_reply(rpl_end_of_list, &name);

		if (msg.empty())
			return true;

		list.push_back(msg);
	}

	return false;
}

/**
 * @brief Sends a RPL_CHANNELMODEIS reply to a client.
 *
 * @param client The client to send the reply to.
 * @param name The name of the channel for which modes are to be sent.
 * @param modes The modes that are to be sent.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void reply_channel_mode_is(Client &client, ChannelName const &name, Channel::Modes const &modes)
{
	std::string const modes_as_string = modes.to_string();
	std::string const channel_mode_is = format_reply(RPL_CHANNELMODEIS, &name, &modes_as_string);

	if (channel_mode_is.empty())
		throw ProblemWithFormatReply();

	std::list<std::string> invite_list;

	if (fill_list_of_replies(invite_list, modes.get_invite_masks(), RPL_INVITELIST, RPL_ENDOFINVITELIST, name))
		throw ProblemWithFormatReply();

	std::list<std::string> ban_list;

	if (fill_list_of_replies(ban_list, modes.get_ban_masks(), RPL_BANLIST, RPL_ENDOFBANLIST, name))
		throw ProblemWithFormatReply();

	client.append_to_msg_out(channel_mode_is);
	for (std::list<std::string>::const_iterator cit = invite_list.begin(); cit != invite_list.end(); ++cit)
		client.append_to_msg_out(*cit);
	for (std::list<std::string>::const_iterator cit = ban_list.begin(); cit != ban_list.end(); ++cit)
		client.append_to_msg_out(*cit);
}

/**
 * @brief
 * Adds the InviteMask mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_invite_mask_mode_to_be_set(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be,
	ParsingTools         &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &invite_mask = *parsing_tools.next_word++;

	if (invite_mask.is_valid())
	{
		if (!modes.has_invite_mask(invite_mask))
			modes_to_be[Set].set(InviteMask, &invite_mask);
		modes_to_be[Cleared].clear(InviteMask, &invite_mask);
	}
	return 0;
}

/**
 * @brief
 * Adds the BanMask mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_ban_mask_mode_to_be_set(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be,
	ParsingTools         &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &ban_mask = *parsing_tools.next_word++;

	if (ban_mask.is_valid())
	{
		if (!modes.has_ban_mask(ban_mask))
			modes_to_be[Set].set(BanMask, &ban_mask);
		modes_to_be[Cleared].clear(BanMask, &ban_mask);
	}
	return 0;
}

/**
 * @brief
 * Adds the InviteOnly mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_invite_only_mode_to_be_set(Channel::Modes const &modes, Channel::Modes *const modes_to_be)
{
	if (!modes.is_set(InviteOnly))
		modes_to_be[Set].set(InviteOnly);
	modes_to_be[Cleared].clear(InviteOnly);
}

/**
 * @brief
 * Adds the KeyProtected mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param mdoes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_KEYSET` upon failure due to a key being set whereas a key is already set.
 */
inline static int save_key_mode_to_be_set(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be,
	ParsingTools         &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;
	if (modes.is_set(KeyProtected))
		return ERR_KEYSET;

	Key const &key = *parsing_tools.next_word++;

	if (key.is_valid())
	{
		modes_to_be[Set].set(KeyProtected, &key);
		modes_to_be[Cleared].clear(KeyProtected);
	}
	return 0;
}

/**
 * @brief
 * Adds the Limit mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_limit_mode_to_be_set(Channel::Modes *const modes_to_be, ParsingTools &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	size_t limit;

	std::stringstream(*parsing_tools.next_word++) >> limit;

	if (limit)
	{
		modes_to_be[Set].set(Limit, &limit);
		modes_to_be[Cleared].clear(Limit);
	}
	return 0;
}

/**
 * @brief
 * Adds the NoMessagesFromOutside mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_no_messages_from_outside_mode_to_be_set(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be)
{
	if (!modes.is_set(NoMessagesFromOutside))
		modes_to_be[Set].set(NoMessagesFromOutside);
	modes_to_be[Cleared].clear(NoMessagesFromOutside);
}

/**
 * @brief
 * Adds the ChannelOperator mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param channel The channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 * @param clients_by_nickname The map of clients, indexed by their nicknames.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_operator_mode_to_be_set(
	Channel const                           &channel,
	Channel::Modes *const                    modes_to_be,
	ParsingTools                            &parsing_tools,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &nickname = *parsing_tools.next_word++;

	if (nickname.is_valid())
	{
		std::map<NickName, Client *const>::const_iterator const client_by_nickname = clients_by_nickname.find(nickname);

		if (client_by_nickname == clients_by_nickname.end())
			return ERR_USERNOTONCHANNEL;

		Client &client = *client_by_nickname->second;

		if (!channel.has_member(client))
			return ERR_USERNOTONCHANNEL;

		if (!channel.get_modes().has_operator(client))
			modes_to_be[Set].set(ChannelOperator, &client);
		modes_to_be[Cleared].clear(ChannelOperator, &client);
	}
	return 0;
}

/**
 * @brief
 * Adds the RestrictedTopic mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param modes The current modes of the channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_restricted_topic_mode_to_be_set(Channel::Modes const &modes, Channel::Modes *const modes_to_be)
{
	if (!modes.is_set(RestrictedTopic))
		modes_to_be[Set].set(RestrictedTopic);
	modes_to_be[Cleared].clear(RestrictedTopic);
}

/**
 * @brief
 * Adds a channel mode to the list of channel modes to be set + Removes it from the list of channel modes to be cleared.
 *
 * @param channel The channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 * @param clients_by_nickname The map of clients, indexed by their nicknames.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_KEYSET` upon failure due to a key being set whereas a key is already set.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 */
inline static int save_channel_mode_to_be_set(
	Channel const                           &channel,
	Channel::Modes *const                    modes_to_be,
	ParsingTools                            &parsing_tools,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	ChannelModeIterator const cit = channel_modes.find(*parsing_tools.current_character);

	if (cit == channel_modes.end())
		return ERR_UNKNOWNMODE;

	ChannelMode const &mode = cit->second;

	switch (mode)
	{
	case InviteMask:
		return save_invite_mask_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
	case BanMask:
		return save_ban_mask_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
	case InviteOnly:
		save_invite_only_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	case KeyProtected:
		return save_key_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
	case Limit:
		return save_limit_mode_to_be_set(modes_to_be, parsing_tools);
	case NoMessagesFromOutside:
		save_no_messages_from_outside_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	case ChannelOperator:
		return save_operator_mode_to_be_set(channel, modes_to_be, parsing_tools, clients_by_nickname);
	case RestrictedTopic:
		save_restricted_topic_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	}
	return 0;
}

/**
 * @brief
 * Adds the InviteMask mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_invite_mask_mode_to_be_cleared(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be,
	ParsingTools         &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &invite_mask = *parsing_tools.next_word++;

	if (invite_mask.is_valid())
	{
		if (modes.has_invite_mask(invite_mask))
			modes_to_be[Cleared].set(InviteMask, &invite_mask);
		modes_to_be[Set].clear(InviteMask, &invite_mask);
	}
	return 0;
}

/**
 * @brief
 * Adds the BanMask mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_ban_mask_mode_to_be_cleared(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be,
	ParsingTools         &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &ban_mask = *parsing_tools.next_word++;

	if (ban_mask.is_valid())
	{
		if (modes.has_ban_mask(ban_mask))
			modes_to_be[Cleared].set(BanMask, &ban_mask);
		modes_to_be[Set].clear(BanMask, &ban_mask);
	}
	return 0;
}

/**
 * @brief
 * Adds the InviteOnly mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_invite_only_mode_to_be_cleared(Channel::Modes const &modes, Channel::Modes *const modes_to_be)
{
	if (modes.is_set(InviteOnly))
		modes_to_be[Cleared].set(InviteOnly);
	modes_to_be[Set].clear(InviteOnly);
}

/**
 * @brief
 * Adds the KeyProtected mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_key_mode_to_be_cleared(Channel::Modes const &modes, Channel::Modes *const modes_to_be)
{
	if (modes.is_set(KeyProtected))
		modes_to_be[Cleared].set(KeyProtected);
	modes_to_be[Set].clear(KeyProtected);
}

/**
 * @brief
 * Adds the Limit mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_limit_mode_to_be_cleared(Channel::Modes const &modes, Channel::Modes *const modes_to_be)
{
	if (modes.is_set(Limit))
		modes_to_be[Cleared].set(Limit);
	modes_to_be[Set].clear(Limit);
}

/**
 * @brief
 * Adds the NoMessagesFromOutside mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_no_messages_from_outside_mode_to_be_cleared(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be)
{
	if (modes.is_set(NoMessagesFromOutside))
		modes_to_be[Cleared].set(NoMessagesFromOutside);
	modes_to_be[Set].clear(NoMessagesFromOutside);
}

/**
 * @brief
 * Adds the ChannelOperator mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param channel The channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 * @param clients_by_nickname The map of clients, indexed by their nicknames.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 */
inline static int save_operator_mode_to_be_cleared(
	Channel const                           &channel,
	Channel::Modes *const                    modes_to_be,
	ParsingTools                            &parsing_tools,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &nickname = *parsing_tools.next_word++;

	if (nickname.is_valid())
	{
		std::map<NickName, Client *const>::const_iterator const client_by_nickname = clients_by_nickname.find(nickname);

		if (client_by_nickname == clients_by_nickname.end())
			return ERR_USERNOTONCHANNEL;

		Client &client = *client_by_nickname->second;

		if (!channel.has_member(client))
			return ERR_USERNOTONCHANNEL;

		if (channel.get_modes().has_operator(client))
			modes_to_be[Cleared].set(ChannelOperator, &client);
		modes_to_be[Set].clear(ChannelOperator, &client);
	}
	return 0;
}

/**
 * @brief
 * Adds the RestrictedTopic mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param modes The current modes of the channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void save_restricted_topic_mode_to_be_cleared(
	Channel::Modes const &modes,
	Channel::Modes *const modes_to_be)
{
	if (modes.is_set(RestrictedTopic))
		modes_to_be[Cleared].set(RestrictedTopic);
	modes_to_be[Set].clear(RestrictedTopic);
}

/**
 * @brief
 * Adds a channel mode to the list of channel modes to be cleared + Removes it from the list of channel modes to be
 * set.
 *
 * @param channel The channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 * @param clients_by_nickname The map of clients, indexed by their nicknames.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 */
inline static int save_channel_mode_to_be_cleared(
	Channel const                           &channel,
	Channel::Modes *const                    modes_to_be,
	ParsingTools                            &parsing_tools,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	ChannelModeIterator const cit = channel_modes.find(*parsing_tools.current_character);

	if (cit == channel_modes.end())
		return ERR_UNKNOWNMODE;

	ChannelMode const &mode = cit->second;

	switch (mode)
	{
	case InviteMask:
		return save_invite_mask_mode_to_be_cleared(channel.get_modes(), modes_to_be, parsing_tools);
	case BanMask:
		return save_ban_mask_mode_to_be_cleared(channel.get_modes(), modes_to_be, parsing_tools);
	case InviteOnly:
		save_invite_only_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case KeyProtected:
		save_key_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case Limit:
		save_limit_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case NoMessagesFromOutside:
		save_no_messages_from_outside_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case ChannelOperator:
		return save_operator_mode_to_be_cleared(channel, modes_to_be, parsing_tools, clients_by_nickname);
	case RestrictedTopic:
		save_restricted_topic_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	}
	return 0;
}

/**
 * @brief Saves the changes to be made to the modes of a channel in two lists of modes.
 *
 * @param channel The channel for which the modes shall change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 * @param clients_by_nickname The map of clients, indexed by their nicknames.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_KEYSET` upon failure due to a key being set whereas a key is already set.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 */
inline static int save_changes_for_channel(
	Channel const                           &channel,
	Channel::Modes *const                    modes_to_be,
	ParsingTools                            &parsing_tools,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	for (parsing_tools.next_word = parsing_tools.current_word + 1;
	     parsing_tools.current_word != parsing_tools.end_of_words;
	     parsing_tools.current_word = parsing_tools.next_word++)
	{
		for (parsing_tools.current_character = parsing_tools.current_word->begin();
		     parsing_tools.current_character < parsing_tools.current_word->end();)
		{
			int ret;

			switch (*parsing_tools.current_character)
			{
			case '+':
				do
				{
					++parsing_tools.current_character;
					ret = save_channel_mode_to_be_set(channel, modes_to_be, parsing_tools, clients_by_nickname);
				}
				while (!ret);
				break;
			case '-':
				do
				{
					++parsing_tools.current_character;
					ret = save_channel_mode_to_be_cleared(channel, modes_to_be, parsing_tools, clients_by_nickname);
				}
				while (!ret);
				break;
			default:
				return ERR_UNKNOWNMODE;
			}
			if (ret != ERR_UNKNOWNMODE)
				return ret;
		}
	}
	return 0;
}

/**
 * @brief Sends an ERR_UNKNWONMODE reply to a client.
 *
 * @param client The client to send the reply to.
 * @param name The name of the channel for which the mode is unknown.
 * @param unknown The unknown mode character.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_unknown_mode(Client &client, ChannelName const &name, char const unknown)
{
	std::string const msg = format_reply(ERR_UNKNOWNMODE, unknown, &name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Sends an ERR_KEYSET reply to a client.
 *
 * @param client The client to send the reply to.
 * @param name The name of the channel for which the key is already set.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_key_set(Client &client, ChannelName const &name)
{
	std::string const msg = format_reply(ERR_KEYSET, &name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Applies the changes of the InviteMask mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_invite_mask_changes_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(InviteMask))
	{
		std::set<NickName> const &invite_masks = modes_to_be[Set].get_invite_masks();

		for (std::set<NickName>::const_iterator cit = invite_masks.begin(); cit != invite_masks.end(); ++cit)
			channel.set_mode(InviteMask, &*cit);
	}
	else if (modes_to_be[Cleared].is_set(InviteMask))
	{
		std::set<NickName> const &invite_masks = modes_to_be[Cleared].get_invite_masks();

		for (std::set<NickName>::const_iterator cit = invite_masks.begin(); cit != invite_masks.end(); ++cit)
			channel.clear_mode(InviteMask, &*cit);
	}
}

/**
 * @brief Applies the changes of the BanMask mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_ban_mask_changes_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(BanMask))
	{
		std::set<NickName> const &ban_masks = modes_to_be[Set].get_ban_masks();

		for (std::set<NickName>::const_iterator cit = ban_masks.begin(); cit != ban_masks.end(); ++cit)
			channel.set_mode(BanMask, &*cit);
	}
	else if (modes_to_be[Cleared].is_set(BanMask))
	{
		std::set<NickName> const &ban_masks = modes_to_be[Cleared].get_ban_masks();

		for (std::set<NickName>::const_iterator cit = ban_masks.begin(); cit != ban_masks.end(); ++cit)
			channel.clear_mode(BanMask, &*cit);
	}
}

/**
 * @brief Applies the change of the InviteOnly mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_invite_only_change_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(InviteOnly))
		channel.set_mode(InviteOnly);
	else if (modes_to_be[Cleared].is_set(InviteOnly))
		channel.clear_mode(InviteOnly);
}

/**
 * @brief Applies the change of the KeyProtected mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_key_change_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(KeyProtected))
		channel.set_mode(KeyProtected, &modes_to_be[Set].get_key());
	else if (modes_to_be[Cleared].is_set(KeyProtected))
		channel.clear_mode(KeyProtected);
}

/**
 * @brief Applies the change of the Limit mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_limit_change_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(Limit))
		channel.set_mode(Limit, &modes_to_be[Set].get_limit());
	else if (modes_to_be[Cleared].is_set(Limit))
		channel.clear_mode(Limit);
}

/**
 * @brief Applies the change of the NoMessagesFromOutside mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_no_messages_from_outside_change_for_channel(
	Channel                    &channel,
	Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(NoMessagesFromOutside))
		channel.set_mode(NoMessagesFromOutside);
	else if (modes_to_be[Cleared].is_set(NoMessagesFromOutside))
		channel.clear_mode(NoMessagesFromOutside);
}

/**
 * @brief Applies the changes of the ChannelOperator mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_operator_changes_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(ChannelOperator))
	{
		std::set<Client const *> const &operators = modes_to_be[Set].get_operators();

		for (std::set<Client const *>::const_iterator cit = operators.begin(); cit != operators.end(); ++cit)
			channel.set_mode(ChannelOperator, *cit);
	}
	else if (modes_to_be[Cleared].is_set(ChannelOperator))
	{
		std::set<Client const *> const &operators = modes_to_be[Cleared].get_operators();

		for (std::set<Client const *>::const_iterator cit = operators.begin(); cit != operators.end(); ++cit)
			channel.clear_mode(ChannelOperator, *cit);
	}
}

/**
 * @brief Applies the change of the RestrictedTopic mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_restricted_topic_change_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	if (modes_to_be[Set].is_set(RestrictedTopic))
		channel.set_mode(RestrictedTopic);
	else if (modes_to_be[Cleared].is_set(RestrictedTopic))
		channel.clear_mode(RestrictedTopic);
}

/**
 * @brief Applies changes to the modes of a channel.
 *
 * @param channel The channel for which the modes are about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_changes_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	apply_invite_mask_changes_for_channel(channel, modes_to_be);
	apply_ban_mask_changes_for_channel(channel, modes_to_be);
	apply_invite_only_change_for_channel(channel, modes_to_be);
	apply_key_change_for_channel(channel, modes_to_be);
	apply_limit_change_for_channel(channel, modes_to_be);
	apply_no_messages_from_outside_change_for_channel(channel, modes_to_be);
	apply_operator_changes_for_channel(channel, modes_to_be);
	apply_restricted_topic_change_for_channel(channel, modes_to_be);
}

/**
 * @brief Gets the current modes of a channel, or applies some changes to the mode of a channel.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 * @param channels_by_name The map of channels, indexed by their names.
 *
 * @throws `ProblemWithFormatReply` if a reply message cannot be formatted.
 */
inline static void mode_for_channel(
	Client                                  &sender,
	std::vector<std::string> const          &parameters,
	std::map<ChannelName, Channel>          &channels_by_name,
	std::map<NickName, Client *const> const &clients_by_nickname)
{
	ChannelName const                             &name = parameters[0];
	std::map<ChannelName, Channel>::iterator const it = channels_by_name.find(name);

	if (it == channels_by_name.end())
		return error_no_such_channel(sender, name);

	if (name[0] == '+')
		return error_no_channel_modes(sender, name);

	Channel &channel = it->second;

	if (!channel.get_modes().has_operator(sender))
		return error_channel_operator_privileges_needed(sender, name);

	if (parameters.size() == 1)
		return reply_channel_mode_is(sender, name, channel.get_modes());

	Channel::Modes modes_to_be[2];
	ParsingTools   parsing_tools(parameters.begin() + 1, parameters.end());
	int const      ret = save_changes_for_channel(channel, modes_to_be, parsing_tools, clients_by_nickname);

	switch (ret)
	{
	case ERR_USERNOTONCHANNEL:
		return error_user_not_on_channel(sender, name, *parsing_tools.next_word);
	case ERR_NEEDMOREPARAMS:
		return error_need_more_arguments(sender, "MODE");
	case ERR_KEYSET:
		return error_key_set(sender, name);
	case ERR_UNKNOWNMODE:
		return error_unknown_mode(sender, name, *parsing_tools.current_character);
	}

	apply_changes_for_channel(channel, modes_to_be);

	std::string const msg = "MODE " + name + " +" + modes_to_be[Set].to_string(true, true, true) + " -"
	                      + modes_to_be[Cleared].to_string(true, true, true);

	channel.broadcast_to_all_members(msg);
}

//--------------------------------------------------------------------------------------------------------------------+
//                                                        MODE                                                        |
//--------------------------------------------------------------------------------------------------------------------+

/**
 * @brief Gets the current modes of a user/channel, or applies some changes to the mode of a user/channel.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throws `ProblemWithFormatReply` if a reply message cannot be formatted.
 */
void Server::mode(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
		return error_need_more_arguments(sender, "MODE");

	if (std::string("#&+!").find(parameters[0][0]) == std::string::npos)
		return mode_for_user(sender, parameters);
	return mode_for_channel(sender, parameters, this->_channels_by_name, this->_clients_by_nickname);
}
