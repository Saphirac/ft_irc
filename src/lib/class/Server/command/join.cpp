/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 08:10:45 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <cstdio>
#include <list>
#include <utility>

#define MAX_CHANNELS 42

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
inline static Channel *__join_new_channel(
	Client                         &sender,
	ChannelName const              &chan_name,
	std::map<ChannelName, Channel> &channels_by_name)
{
	if (sender.get_joined_channels_by_name().size() == MAX_CHANNELS)
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
 * @param sender
 * @param channel
 * @param key
 * @return the joined channel if successful, NULL otherwise
 */
inline static Channel *__join_existing_channel(
	Client            &sender,
	Channel           &channel,
	ChannelName const &chan_name,
	Key const         &key)
{
	NickName const &nickname = sender.get_nickname();

	if (channel.has_member(sender) == false)
	{
		Channel::Modes const &channel_modes = channel.get_modes();
		size_t                user_limit = channel_modes.get_limit();

		if (user_limit != 0 && user_limit == channel.get_members_size())
			sender.append_formatted_reply_to_msg_out(ERR_CHANNELISFULL, &chan_name);
		else if (channel_modes.is_set(InviteOnly) && !channel_modes.has_invite_mask(nickname))
			sender.append_formatted_reply_to_msg_out(ERR_INVITEONLYCHAN, &chan_name);
		else if (sender.get_joined_channels_by_name().size() == MAX_CHANNELS)
			sender.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
		else if (channel_modes.has_ban_mask(nickname))
			sender.append_formatted_reply_to_msg_out(ERR_BANNEDFROMCHAN, &chan_name);
		else if (
			channel_modes.is_set(KeyProtected)
			&& (!channel_modes.has_invite_mask(nickname) || channel_modes.get_key() != key))
			sender.append_formatted_reply_to_msg_out(ERR_BADCHANNELKEY, &chan_name);
		else
		{
			channel.add_member(sender);
			sender.join_channel(chan_name, channel);
			return &channel;
		}
	}
	return NULL;
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
		return sender.append_to_msg_out(':' + this->_name + " You are not registered.\n");
	if (params.size() < 1)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "JOIN");
	if (params.size() == 1 && params[0] == "0")
	{
		std::vector<std::string> channel_and_msg;

		for (std::map<ChannelName, Channel *const>::const_iterator it = sender.get_joined_channels_by_name().begin();
		     it != sender.get_joined_channels_by_name().end();
		     ++it)
			channel_and_msg.push_back(it->first);
		channel_and_msg.push_back(DEFAULT_QUIT_MSG);
		_part(sender, channel_and_msg);
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
				? __join_new_channel(sender, chan_name, this->_channels_by_name)
				: __join_existing_channel(sender, chan_by_name->second, chan_name, channel_key_pairs[i].second);
		if (new_joined_channel)
			joined_channels.push_back(std::make_pair(chan_name, new_joined_channel));
	}
	__send_join_message_for_each_channel(joined_channels, sender);
}
