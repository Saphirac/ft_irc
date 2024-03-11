/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 15:58:17 by jodufour         ###   ########.fr       */
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
 * @brief Makes a user create and join a new channel.
 *
 * @param user the client sending the JOIN command
 * @param chan_name the name of the channel to join
 * @param channels_by_name the map of channels by name
 *
 * @return The address of the joined channed upon success, `NULL` otherwise.
 */
inline static Channel const *join_new_channel(
	Client                         &user,
	ChannelName const              &chan_name,
	std::map<ChannelName, Channel> &channels_by_name)
{
	if (user.joined_channel_count() == MAXIMUM_NUMBER_OF_JOINED_CHANNELS_BY_USER)
	{
		user.append_formatted_reply_to_msg_out(ERR_TOOMANYCHANNELS);
		return NULL;
	}
	if (!chan_name.is_valid())
	{
		user.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &chan_name);
		return NULL;
	}

	Channel &channel =
		channels_by_name.insert(std::make_pair(chan_name, Channel(chan_name.are_modes_supported()))).first->second;

	user.join_channel(chan_name, channel);
	channel.add_member(user);
	channel.set_mode(ChannelOperator, &user);
	return &channel;
}

/**
 * @brief Makes a user join an existing channel.
 *
 * @param user
 * @param channel
 * @param key
 *
 * @return The address of the joined channel upon success, `NULL` otherwise.
 */
inline static Channel const *join_existing_channel(
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

/**
 * @brief Create a list of pairs of channels and keys
 *
 * @param channels
 * @param keys
 *
 * @return The list of pairs of channels and keys.
 */
inline static std::vector<PairChannelNameKey> split_channels_keys(std::string const &channels, std::string const &keys)
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
		split_channels_keys(params[0], params.size() > 1 ? params[1] : std::string());

	for (size_t i = 0; i < channel_key_pairs.size(); ++i)
	{
		ChannelName const                       &channel_name = channel_key_pairs[i].first;
		std::map<ChannelName, Channel>::iterator channel_by_name = this->_channels_by_name.find(channel_name);
		Channel const *const                     channel =
            channel_by_name == this->_channels_by_name.end()
									? join_new_channel(sender, channel_name, this->_channels_by_name)
									: join_existing_channel(sender, channel_by_name->second, channel_name, channel_key_pairs[i].second);

		if (!channel)
			continue;
		sender.append_to_msg_out(sender.prefix() + "JOIN " + channel_name);
		sender.append_formatted_reply_to_msg_out(RPL_TOPIC, &channel_name, &channel->get_topic());

		std::vector<std::string> names_arguments;

		names_arguments.push_back(channel_name);
		this->_names(sender, names_arguments);
	}
}
