/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 00:20:08 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

#define MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER 42
#define MAXIMUM_NUMBER_OF_MEMBERS_BY_CHANNEL      42

typedef std::list<ChannelName> ChannelNameList;
typedef std::list<Key>         KeyList;

/**
 * @brief Creates a new channel and makes a user join it, promoting them to channel operator.
 *
 * @param user The user that will join the channel.
 * @param channel_name The name of the channel to create and join.
 * @param channels_by_name The map of the existing channels.
 *
 * @return The address of the joined channed upon success, `NULL` otherwise.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static Channel *join_new_channel(
	Client             &user,
	ChannelName const  &channel_name,
	Server::ChannelMap &channels_by_name)
{
	if (!channel_name.is_valid())
	{
		user.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_name);
		return NULL;
	}

	Channel &channel =
		channels_by_name.insert(std::make_pair(channel_name, Channel(channel_name.are_modes_supported())))
			.first->second;

	channel.set_mode(ChannelOperator, &user);
	return &channel;
}

/**
 * @brief Makes a user join an existing channel.
 *
 * @param user The user that will join the channel.
 * @param channel_name The name of the channel to join.
 * @param channel The channel to join.
 * @param key The key provided by the user to join the channel.
 *
 * @return The address of the joined channel upon success, `NULL` otherwise.
 */
inline static Channel *join_existing_channel(
	Client            &user,
	ChannelName const &channel_name,
	Channel           &channel,
	Key const         &key)
{
	if (channel.has_member(user))
		return &channel;

	size_t const member_count = channel.member_count();

	if (channel.get_are_modes_supported())
	{
		NickName const       &nickname = user.get_nickname();
		Channel::Modes const &channel_modes = channel.get_modes();

		if (channel_modes.has_ban_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_BANNEDFROMCHAN, &channel_name);
			return NULL;
		}
		if (channel_modes.is_set(InviteOnly) && !channel.has_invited_user_by_operator(user)
		    && !channel_modes.has_invite_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_INVITEONLYCHAN, &channel_name);
			return NULL;
		}
		if (channel_modes.is_set(KeyProtected) && channel_modes.get_key() != key && !channel.has_invited_user(user)
		    && !channel_modes.has_invite_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_BADCHANNELKEY, &channel_name);
			return NULL;
		}
		if (channel_modes.is_set(Limit) && member_count == channel_modes.get_limit())
		{
			user.append_formatted_reply_to_msg_out(ERR_CHANNELISFULL, &channel_name);
			return NULL;
		}
	}
	if (member_count == MAXIMUM_NUMBER_OF_MEMBERS_BY_CHANNEL)
	{
		user.append_formatted_reply_to_msg_out(ERR_CHANNELISFULL, &channel_name);
		return NULL;
	}

	channel.remove_invited_user(user);
	return &channel;
}

/**
 * @brief Makes a user either join an existing channel or create and join a new channel.
 *
 * @param user The user that will join the channel.
 * @param channel_name The name of the channel to join or create and join.
 * @param key The key provided by the user to join the channel if any.
 *
 * @return `true` upon success, `false` otherwise.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static bool join_channel(
	Server::ChannelMap &channels_by_name,
	Client             &user,
	ChannelName const  &channel_name,
	Key const          &key = Key())
{
	Server::ChannelMap::iterator const channel_by_name = channels_by_name.find(channel_name);
	Channel                           *channel;

	if (channel_by_name == channels_by_name.end())
		channel = join_new_channel(user, channel_name, channels_by_name);
	else
		channel = join_existing_channel(user, channel_name, channel_by_name->second, key);

	if (!channel)
		return false;

	channel->add_member(user);
	user.join_channel(channel_name, *channel);
	channel->broadcast_to_all_members(user.prefix() + "JOIN :" + channel_name);
	user.append_formatted_reply_to_msg_out(RPL_TOPIC, &channel_name, &channel->get_topic());

	return true;
}

/**
 * @brief Makes a user either join existing channel(s) or create and join new channel(s).
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_join(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "JOIN");

	if (parameters[0] == "0")
		return this->_make_user_leave_all_their_joined_channels(sender);

	ChannelNameList const           channel_names = split<ChannelNameList>(parameters[0], ',');
	ChannelNameList::const_iterator channel_name = channel_names.begin();
	KeyList const                   keys = parameters.size() > 1 ? split<KeyList>(parameters[1], ',') : KeyList();

	for (KeyList::const_iterator key = keys.begin(); channel_name != channel_names.end() && key != keys.end();
	     ++channel_name, ++key)
	{
		if (sender.joined_channel_count() == MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER)
		{
			while (channel_name++ != channel_names.end()) sender.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
			return;
		}

		if (!join_channel(this->_channels_by_name, sender, *channel_name, *key))
			continue;

		CommandParameterVector names_arguments(1);

		names_arguments[0] = *channel_name;
		this->_names(sender, names_arguments);
	}
	for (; channel_name != channel_names.end(); ++channel_name)
	{
		if (sender.joined_channel_count() == MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER)
		{
			while (channel_name++ != channel_names.end()) sender.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
			return;
		}

		if (!join_channel(this->_channels_by_name, sender, *channel_name))
			continue;

		CommandParameterVector names_arguments(1);

		names_arguments[0] = *channel_name;
		this->_names(sender, names_arguments);
	}
}
