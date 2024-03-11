/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 12:49:52 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <cstdio>
#include <list>
#include <utility>

#define MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER 42
#define MAXIMUM_NUMBER_OF_MEMBERS_BY_CHANNEL      42

typedef std::pair<ChannelName, Key>       PairChannelNameKey;
typedef std::pair<ChannelName, Channel *> PairChannelNameChannel;
typedef std::vector<ChannelName>          ChannelNameVector;
typedef std::vector<Key>                  KeyVector;

/**
 * @brief Join a new channel
 *
 * @param sender the client sending the JOIN command
 * @param chan_name the name of the channel to join
 * @param channels_by_name the map of channels by name
 * @return the joined channel if successful, NULL otherwise
 */
inline static Channel *join_new_channel(
	Client                         &sender,
	ChannelName const              &chan_name,
	std::map<ChannelName, Channel> &channels_by_name)
{
	if (sender.joined_channel_count() == MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER)
		sender.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
	else if (chan_name.is_valid())
	{
		Channel &channel = channels_by_name[chan_name];

		sender.join_channel(chan_name, channel);
		return &channel;
	}
	else
		sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &chan_name);
	return NULL;
}

/**
 * @brief Join an existing channel
 *
 * @param user
 * @param channel
 * @param key
 *
 * @return the joined channel if successful, NULL otherwise
 */
inline static Channel *join_existing_channel(
	Client            &user,
	Channel           &channel,
	ChannelName const &chan_name,
	Key const         &key)
{
	if (user.joined_channel_count() == MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER)
	{
		user.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
		return NULL;
	}

	size_t const member_count = channel.member_count();

	if (channel.get_are_modes_supported())
	{
		NickName const       &nickname = user.get_nickname();
		Channel::Modes const &channel_modes = channel.get_modes();

		if (channel_modes.has_ban_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_BANNEDFROMCHAN, &chan_name);
			return NULL;
		}
		if (channel_modes.is_set(InviteOnly) && !channel.has_invited_user_by_operator(user)
		    && !channel_modes.has_invite_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_INVITEONLYCHAN, &chan_name);
			return NULL;
		}
		if (channel_modes.is_set(KeyProtected) && channel_modes.get_key() != key && !channel.has_invited_user(user)
		    && !channel_modes.has_invite_mask(nickname))
		{
			user.append_formatted_reply_to_msg_out(ERR_BADCHANNELKEY, &chan_name);
			return NULL;
		}
		if (channel_modes.is_set(Limit) && member_count == channel_modes.get_limit())
		{
			user.append_formatted_reply_to_msg_out(ERR_CHANNELISFULL, &chan_name);
			return NULL;
		}
	}
	if (member_count == MAXIMUM_NUMBER_OF_MEMBERS_BY_CHANNEL)
	{
		user.append_formatted_reply_to_msg_out(ERR_CHANNELISFULL, &chan_name);
		return NULL;
	}

	channel.add_member(user);
	channel.remove_invited_user(user);
	user.join_channel(chan_name, channel);
	return &channel;
}

#include <iostream>

/**
 * @brief Create a list of pairs of channels and keys
 *
 * @param channels
 * @param keys
 * @return std::vector<std::pair<std::string, std::string>>
 */
inline static std::vector<PairChannelNameKey> __split_channels_keys(
	std::string const &channels,
	std::string const &keys)
{
	std::vector<PairChannelNameKey> channel_key_pairs;
	ChannelNameVector               split_channels = split<ChannelNameVector>(channels, ',');
	KeyVector                       split_keys = split<KeyVector>(keys, ',');
	size_t const                    split_keys_size = split_keys.size();
	size_t const                    split_channels_size = split_channels.size();

	for (size_t i = 0; i < split_channels_size; ++i)
		channel_key_pairs.push_back(std::make_pair(split_channels[i], i < split_keys_size ? split_keys[i] : Key()));

	return channel_key_pairs;
}

/**
 * @brief Send the JOIN msg for each channel in the end
 *
 * @param joined_channels
 * @param sender
 */
inline static void __send_join_message_for_each_channel(
	std::list<PairChannelNameChannel> const &joined_channels,
	Client                                  &sender)
{
	for (std::list<PairChannelNameChannel>::const_iterator channel = joined_channels.begin();
	     channel != joined_channels.end();
	     ++channel)
	{
		ChannelName const &chan_name = channel->first;

		sender.append_to_msg_out(sender.prefix() + "JOIN : Successfully joined " + chan_name);
		sender.append_formatted_reply_to_msg_out(RPL_TOPIC, &chan_name, &channel->second->get_topic());
		// names(sender, {chan_name});
	}
}

/**
 * @brief Add the sender to the channels in params if they exist, create them otherwise
 * The second params if present is the key to join the channel
 *
 * @param sender
 * @param params
 */
void Server::_join(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (params.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "JOIN");

	if (params[0] == "0")
	{
		std::vector<std::string>            part_arguments;
		Client::JoinedChannelMap const      joined_channels_by_name = sender.get_joined_channels_by_name();
		Client::JoinedChannelIterator const end = joined_channels_by_name.end();

		for (Client::JoinedChannelIterator joined_channel_by_name = joined_channels_by_name.begin();
		     joined_channel_by_name != end;
		     ++joined_channel_by_name)
			part_arguments.push_back(joined_channel_by_name->first);
		part_arguments.push_back(DEFAULT_QUIT_MESSAGE);

		return this->_part(sender, part_arguments);
	}

	std::vector<PairChannelNameKey> const channel_key_pairs =
		__split_channels_keys(params[0], params.size() > 1 ? params[1] : std::string());
	std::list<PairChannelNameChannel> joined_channels;

	for (size_t i = 0; i < channel_key_pairs.size(); ++i)
	{
		ChannelName const                       &chan_name = channel_key_pairs[i].first;
		std::map<ChannelName, Channel>::iterator chan_by_name = this->_channels_by_name.find(chan_name);

		Channel *const new_joined_channel =
			chan_by_name == this->_channels_by_name.end()
				? join_new_channel(sender, chan_name, this->_channels_by_name)
				: join_existing_channel(sender, chan_by_name->second, chan_name, channel_key_pairs[i].second);
		if (new_joined_channel)
			joined_channels.push_back(std::make_pair(chan_name, new_joined_channel));
	}
	__send_join_message_for_each_channel(joined_channels, sender);
}
