/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/14 02:41:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include <list>
#include <sstream>

typedef std::pair<char, UserMode>    UserModePair;
typedef std::map<char, UserMode>     UserModeMap;
typedef std::pair<char, ChannelMode> ChannelModePair;
typedef std::map<char, ChannelMode>  ChannelModeMap;

static UserModePair const raw_user_modes_by_identifier[] = {
	std::make_pair(USER_MODES[Bot], Bot),
	std::make_pair(USER_MODES[LocalOperator], LocalOperator),
	std::make_pair(USER_MODES[Away], Away),
	std::make_pair(USER_MODES[Invisible], Invisible),
	std::make_pair(USER_MODES[WallopsListener], WallopsListener),
};
static UserModeMap const user_modes_by_identifier(
	raw_user_modes_by_identifier,
	raw_user_modes_by_identifier + sizeof(raw_user_modes_by_identifier) / sizeof(*raw_user_modes_by_identifier));

static ChannelModePair const raw_channel_modes_by_identifier[] = {
	std::make_pair(CHANNEL_MODES[InviteOnly], InviteOnly),
	std::make_pair(CHANNEL_MODES[NoMessagesFromOutside], NoMessagesFromOutside),
	std::make_pair(CHANNEL_MODES[RestrictedTopic], RestrictedTopic),
	std::make_pair(CHANNEL_MODES[InviteMask], InviteMask),
	std::make_pair(CHANNEL_MODES[BanMask], BanMask),
	std::make_pair(CHANNEL_MODES[KeyProtected], KeyProtected),
	std::make_pair(CHANNEL_MODES[Limit], Limit),
	std::make_pair(CHANNEL_MODES[ChannelOperator], ChannelOperator),
};
static ChannelModeMap const channel_modes_by_identifier(
	raw_channel_modes_by_identifier,
	raw_channel_modes_by_identifier
		+ sizeof(raw_channel_modes_by_identifier) / sizeof(*raw_channel_modes_by_identifier));

enum
{
	Set,
	Cleared
};

struct ParsingTools
{
	Server::CommandParameterVector::const_iterator       current_word;
	Server::CommandParameterVector::const_iterator       next_word;
	Server::CommandParameterVector::const_iterator const end_of_words;
	std::string::const_iterator                          current_character;

	// Constructors
	ParsingTools(
		Server::CommandParameterVector::const_iterator const &current_word,
		Server::CommandParameterVector::const_iterator const &end_of_words) :
		current_word(current_word),
		next_word(current_word + 1),
		end_of_words(end_of_words),
		current_character(current_word->begin())
	{
	}

	// Destructor
	~ParsingTools(){};
};

#pragma region USER MODE
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
	UserModeMap::const_iterator const user_mode_by_identifier = user_modes_by_identifier.find(identifer);

	if (user_mode_by_identifier == user_modes_by_identifier.end())
		return false;

	UserMode const &mode = user_mode_by_identifier->second;

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
	UserModeMap::const_iterator const user_mode_by_identifier = user_modes_by_identifier.find(identifer);

	if (user_mode_by_identifier == user_modes_by_identifier.end())
		return false;

	UserMode const &mode = user_mode_by_identifier->second;

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
 * @brief Applies changes to the modes of a user.
 *
 * @param client The Client instance to apply the changes to.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 */
inline static void apply_changes_for_user(Client &client, Client::Modes *const modes_to_be)
{
	if (modes_to_be[Set].is_set(Bot))
		client.set_mode(Bot);
	else if (modes_to_be[Cleared].is_set(Bot))
		client.clear_mode(Bot);

	if (modes_to_be[Set].is_set(LocalOperator))
		client.set_mode(LocalOperator);
	else if (modes_to_be[Cleared].is_set(LocalOperator))
		client.clear_mode(LocalOperator);

	if (modes_to_be[Set].is_set(WallopsListener))
		client.set_mode(WallopsListener);
	else if (modes_to_be[Cleared].is_set(WallopsListener))
		client.clear_mode(WallopsListener);

	if (modes_to_be[Set].is_set(Invisible))
		client.set_mode(Invisible);
	else if (modes_to_be[Cleared].is_set(Invisible))
		client.clear_mode(Invisible);
}

/**
 * @brief Gets the current modes of a user, or applies some changes to the mode of a user.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static void mode_for_user(Client &sender, Server::CommandParameterVector const &parameters)
{
	NickName const &nickname = parameters[0];

	if (nickname != sender.get_nickname())
		return sender.append_formatted_reply_to_msg_out(ERR_USERSDONTMATCH);

	if (parameters.size() == 1)
	{
		std::string const modes_as_string = sender.get_modes().to_string();

		return sender.append_formatted_reply_to_msg_out(RPL_UMODEIS, &modes_as_string);
	}

	Client::Modes modes_to_be[2];

	if (save_changes_for_user(sender.get_modes(), modes_to_be, parameters[1]))
		return sender.append_formatted_reply_to_msg_out(ERR_UMODEUNKNOWNFLAG);

	apply_changes_for_user(sender, modes_to_be);

	bool const has_any_modes_to_be_set = modes_to_be[Set].has_any_mode_set();
	bool const has_any_modes_to_be_cleared = modes_to_be[Cleared].has_any_mode_set();

	if (!has_any_modes_to_be_set && !has_any_modes_to_be_cleared)
		return;

	std::string msg = sender.prefix() + "MODE " + sender.get_nickname();

	if (modes_to_be[Set].has_any_mode_set())
		msg += " +" + modes_to_be[Set].to_string();
	if (modes_to_be[Cleared].has_any_mode_set())
		msg += " -" + modes_to_be[Cleared].to_string();

	sender.append_to_msg_out(msg);
}
#pragma endregion USER MODE

#pragma region CHANNEL MODE
/**
 * @brief Sends a RPL_CHANNELMODEIS reply to a client.
 *
 * @param client The client to send the reply to.
 * @param name The name of the channel for which modes are to be sent.
 * @param modes The modes that are to be sent.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static void reply_channel_mode_is(Client &client, ChannelName const &name, Channel::Modes const &modes)
{
	std::string const        modes_as_string = modes.to_string();
	std::set<NickName> const invite_masks = modes.get_invite_masks();

	client.append_formatted_reply_to_msg_out(RPL_CHANNELMODEIS, &name, &modes_as_string);
	if (!invite_masks.empty())
	{
		for (std::set<NickName>::const_iterator cit = invite_masks.begin(); cit != invite_masks.end(); ++cit)
			client.append_formatted_reply_to_msg_out(RPL_INVITELIST, &name, &*cit);
		client.append_formatted_reply_to_msg_out(RPL_ENDOFINVITELIST, &name);
	}

	std::set<NickName> const ban_masks = modes.get_ban_masks();

	if (!ban_masks.empty())
	{
		for (std::set<NickName>::const_iterator cit = ban_masks.begin(); cit != ban_masks.end(); ++cit)
			client.append_formatted_reply_to_msg_out(RPL_BANLIST, &name, &*cit);
		client.append_formatted_reply_to_msg_out(RPL_ENDOFBANLIST, &name);
	}
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
 * Adds the ChannelOperator mode to the list of channel modes to be set
 * +
 * Removes it from the list of channel modes to be cleared.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channel The channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static int save_operator_mode_to_be_set(
	Server::ClientMap const &users_by_nickname,
	Channel const           &channel,
	Channel::Modes *const    modes_to_be,
	ParsingTools            &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &nickname = *parsing_tools.next_word++;

	if (nickname.is_valid())
	{
		Server::ClientMap::const_iterator const user_by_nickname = users_by_nickname.find(nickname);

		if (user_by_nickname == users_by_nickname.end())
			return ERR_USERNOTONCHANNEL;

		Client &user = *user_by_nickname->second;

		if (!channel.has_member(user))
			return ERR_USERNOTONCHANNEL;

		if (!channel.get_modes().has_operator(user))
			modes_to_be[Set].set(ChannelOperator, &user);
		modes_to_be[Cleared].clear(ChannelOperator, &user);
	}
	return 0;
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
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 * Adds a channel mode to the list of channel modes to be set + Removes it from the list of channel modes to be cleared.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channel The channel for which the mode shall be set.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_KEYSET` upon failure due to a key being set whereas a key is already set.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static int save_channel_mode_to_be_set(
	Server::ClientMap const &users_by_nickname,
	Channel const           &channel,
	Channel::Modes *const    modes_to_be,
	ParsingTools            &parsing_tools)
{
	ChannelModeMap::const_iterator const channel_mode_by_identifier =
		channel_modes_by_identifier.find(*parsing_tools.current_character);

	if (channel_mode_by_identifier == channel_modes_by_identifier.end())
		return ERR_UNKNOWNMODE;

	ChannelMode const &mode = channel_mode_by_identifier->second;

	switch (mode)
	{
	case InviteOnly:
		save_invite_only_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	case NoMessagesFromOutside:
		save_no_messages_from_outside_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	case RestrictedTopic:
		save_restricted_topic_mode_to_be_set(channel.get_modes(), modes_to_be);
		break;
	case Limit:
		return save_limit_mode_to_be_set(modes_to_be, parsing_tools);
	case KeyProtected:
		return save_key_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
	case ChannelOperator:
		return save_operator_mode_to_be_set(users_by_nickname, channel, modes_to_be, parsing_tools);
	case InviteMask:
		return save_invite_mask_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
	case BanMask:
		return save_ban_mask_mode_to_be_set(channel.get_modes(), modes_to_be, parsing_tools);
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
 * Adds the ChannelOperator mode to the list of channel modes to be cleared
 * +
 * Removes it from the list of channel modes to be set.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channel The channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static int save_operator_mode_to_be_cleared(
	Server::ClientMap const &users_by_nickname,
	Channel const           &channel,
	Channel::Modes *const    modes_to_be,
	ParsingTools            &parsing_tools)
{
	if (parsing_tools.next_word == parsing_tools.end_of_words)
		return ERR_NEEDMOREPARAMS;

	NickName const &nickname = *parsing_tools.next_word++;

	if (nickname.is_valid())
	{
		Server::ClientMap::const_iterator const user_by_nickname = users_by_nickname.find(nickname);

		if (user_by_nickname == users_by_nickname.end())
			return ERR_USERNOTONCHANNEL;

		Client &user = *user_by_nickname->second;

		if (!channel.has_member(user))
			return ERR_USERNOTONCHANNEL;

		if (channel.get_modes().has_operator(user))
			modes_to_be[Cleared].set(ChannelOperator, &user);
		modes_to_be[Set].clear(ChannelOperator, &user);
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
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 * Adds a channel mode to the list of channel modes to be cleared + Removes it from the list of channel modes to be
 * set.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channel The channel for which the mode shall be cleared.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static int save_channel_mode_to_be_cleared(
	Server::ClientMap const &users_by_nickname,
	Channel const           &channel,
	Channel::Modes *const    modes_to_be,
	ParsingTools            &parsing_tools)
{
	ChannelModeMap::const_iterator const channel_mode_by_identifier =
		channel_modes_by_identifier.find(*parsing_tools.current_character);

	if (channel_mode_by_identifier == channel_modes_by_identifier.end())
		return ERR_UNKNOWNMODE;

	ChannelMode const &mode = channel_mode_by_identifier->second;

	switch (mode)
	{
	case InviteOnly:
		save_invite_only_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case NoMessagesFromOutside:
		save_no_messages_from_outside_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case RestrictedTopic:
		save_restricted_topic_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case Limit:
		save_limit_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case KeyProtected:
		save_key_mode_to_be_cleared(channel.get_modes(), modes_to_be);
		break;
	case ChannelOperator:
		return save_operator_mode_to_be_cleared(users_by_nickname, channel, modes_to_be, parsing_tools);
	case InviteMask:
		return save_invite_mask_mode_to_be_cleared(channel.get_modes(), modes_to_be, parsing_tools);
	case BanMask:
		return save_ban_mask_mode_to_be_cleared(channel.get_modes(), modes_to_be, parsing_tools);
	}
	return 0;
}

/**
 * @brief Saves the changes to be made to the modes of a channel in two lists of modes.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channel The channel for which the modes shall change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 * @param parsing_tools The iterators to use to parse the changes.
 *
 * @return
 * Zero upon success.
 * `ERR_USERNOTONCHANNEL` upon failure due to a channel operator promotion for someone that is not on the channel.
 * `ERR_NEEDMOREPARAMS` upon failure due to a lack of arguments.
 * `ERR_KEYSET` upon failure due to a key being set whereas a key is already set.
 * `ERR_UNKNOWNMODE` upon failure due to a unknown mode character.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static int save_changes_for_channel(
	Server::ClientMap const &users_by_nickname,
	Channel const           &channel,
	Channel::Modes *const    modes_to_be,
	ParsingTools            &parsing_tools)
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
					ret = save_channel_mode_to_be_set(users_by_nickname, channel, modes_to_be, parsing_tools);
				}
				while (!ret);
				break;
			case '-':
				do
				{
					++parsing_tools.current_character;
					ret = save_channel_mode_to_be_cleared(users_by_nickname, channel, modes_to_be, parsing_tools);
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
 * @brief Applies the changes of the ChannelOperator mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 * @brief Applies the changes of the InviteMask mode to the modes of a channel.
 *
 * @param channel The channel for which the mode is about change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
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
 * @brief Applies changes to the modes of a channel.
 *
 * @param channel The channel for which the modes are about to change.
 * @param modes_to_be The two lists of modes that are about to be set and cleared.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static void apply_changes_for_channel(Channel &channel, Channel::Modes const *const modes_to_be)
{
	apply_invite_only_change_for_channel(channel, modes_to_be);
	apply_no_messages_from_outside_change_for_channel(channel, modes_to_be);
	apply_restricted_topic_change_for_channel(channel, modes_to_be);
	apply_limit_change_for_channel(channel, modes_to_be);
	apply_key_change_for_channel(channel, modes_to_be);
	apply_operator_changes_for_channel(channel, modes_to_be);
	apply_invite_mask_changes_for_channel(channel, modes_to_be);
	apply_ban_mask_changes_for_channel(channel, modes_to_be);
}

/**
 * @brief Gets the current modes of a channel, or applies some changes to the mode of a channel.
 *
 * @param users_by_nickname The map of users, indexed by their nickname.
 * @param channels_by_name The map of channels, indexed by their names.
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static void mode_for_channel(
	Server::ClientMap const              &users_by_nickname,
	Server::ChannelMap                   &channels_by_name,
	Client                               &sender,
	Server::CommandParameterVector const &parameters)
{
	ChannelName const                 &name = parameters[0];
	Server::ChannelMap::iterator const it = channels_by_name.find(name);

	if (it == channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &name);

	if (name[0] == '+')
		return sender.append_formatted_reply_to_msg_out(ERR_NOCHANMODES, &name);

	Channel &channel = it->second;

	if (parameters.size() == 1)
		return reply_channel_mode_is(sender, name, channel.get_modes());

	if (!channel.get_modes().has_operator(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &name);

	Channel::Modes modes_to_be[2];
	ParsingTools   parsing_tools(parameters.begin() + 1, parameters.end());
	int const      ret = save_changes_for_channel(users_by_nickname, channel, modes_to_be, parsing_tools);

	switch (ret)
	{
	case ERR_USERNOTONCHANNEL:
		return sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &sender.get_nickname(), &name);
	case ERR_NEEDMOREPARAMS:
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "MODE");
	case ERR_KEYSET:
		return sender.append_formatted_reply_to_msg_out(ERR_KEYSET, &name);
	case ERR_UNKNOWNMODE:
		return sender.append_formatted_reply_to_msg_out(ERR_UNKNOWNMODE, *parsing_tools.current_character, &name);
	}

	bool const is_any_mode_to_be_set = modes_to_be[Set].has_any_mode_set();
	bool const is_any_mode_to_be_cleared = modes_to_be[Cleared].has_any_mode_set();

	if (!is_any_mode_to_be_set && !is_any_mode_to_be_cleared)
		return;

	apply_changes_for_channel(channel, modes_to_be);

	std::string msg = sender.prefix() + "MODE " + name;

	if (is_any_mode_to_be_set)
		msg += " +" + modes_to_be[Set].to_string(true, true, true);
	if (is_any_mode_to_be_cleared)
		msg += " -" + modes_to_be[Cleared].to_string(true, true, true);

	channel.broadcast_to_all_members(msg);
}
#pragma endregion CHANNEL MODE

/**
 * @brief Gets the current modes of a user/channel, or applies some changes to the mode of a user/channel.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if function of the C++ standard library critically fails.
 */
void Server::_mode(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "MODE");

	if (std::string("#&+!").find(parameters[0][0]) == std::string::npos)
		return mode_for_user(sender, parameters);
	return mode_for_channel(this->_clients_by_nickname, this->_channels_by_name, sender, parameters);
}
