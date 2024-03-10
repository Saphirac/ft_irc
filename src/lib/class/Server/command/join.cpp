/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 22:56:35 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>
#include <utility>

#define MAX_CHANNELS 42

/**
 * @brief Join a new channel
 *
 * @param sender
 * @param chan_name
 * @return the joined channel if successful, NULL otherwise
 */
inline static Channel const *__join_new_channel(Client &sender, ChannelName const &chan_name)
{
	if (sender->get_channels.size() > MAX_CHANNELS)
		sender->append_to_msg_out(sender.formatted_reply(ERR_TOOMANYCHANNELS));
	else if (chan_name.is_valid())
	{
		Channel &channel = this->_channels_by_name[chan_name];
		sender->add_channel(channel);
		return channel;
	}
	else
		sender->append_to_msg_out(sender.formatted_reply(ERR_NOSUCHCHANNEL, chan_name));
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
inline static Channel const *__join_existing_channel(Client &sender, Channel &channel, std::string const &key)
{
	std::string const &nickname = sender.get_nickname();

	if (channel->has_member(sender) == false)
	{
		Channel::Modes const &channel_modes = channel->get_modes();
		size_t                user_limit = channel_modes->get_limit();
		ChannelName const    &chan_name = channel->get_name();

		if (user_limit != 0 && user_limit == channel->get_members().size())
			sender->append_to_msg_out(sender.formatted_reply(ERR_CHANNELISFULL, chan_name));
		else if (channel->has_mode(InviteOnly) && !channel_modes.has_invite_mask(nickname))
			sender->append_to_msg_out(sender.formatted_reply(ERR_INVITEONLYCHAN, chan_name));
		else if (sender.get_channels.size() == MAX_CHANNELS)
			sender->append_to_msg_out(sender.formatted_reply(ERR_TOOMANYCHANNELS));
		else if (channel_modes.has_ban_mask(nickname))
			sender->append_to_msg_out(sender.formatted_reply(ERR_BANNEDFROMCHAN, chan_name));
		else if (channel->has_mode(Key) && (!channel.has_invite_mask(nickname) || channel_modes.get_key() != key))
			sender->append_to_msg_out(sender.formatted_reply(ERR_BADCHANNELKEY, chan_name));
		else
		{
			channel->add_member(sender);
			sender->add_channel(channel);
			return channel;
		}
	}
	return NULL;
}

/**
 * @brief Create a list of pairs of channels and keys
 *
 * @param channels
 * @param keys
 * @return std::vector<std::pair<std::string, std::string>>
 */
inline static std::vector<std::pair<std::string, std::string>> __split_channels_keys(
	std::string const &channels,
	std::string const &keys)
{
	std::vector<std::pair<std::string, std::string>> channel_key_pairs;
	std::vector<std::string>                         split_channels = split<std::vector<std::string>>(channels, ',');
	std::vector<std::string>                         split_keys = split<std::vector<std::string>>(keys, ',');
	for (size_t i = 0; i < split_channels.size(); ++i)
	{
		if (split_keys[i].empty())
			channel_key_pairs.push_back(std::make_pair(split_channels[i], std::string()));
		else
			channel_key_pairs.push_back(std::make_pair(split_channels[i], split_keys[i]));
	}
	return channel_key_pairs;
}

/**
 * @brief Send the JOIN msg for each channel in the end
 *
 * @param joined_channels
 * @param sender
 */
inline static void __send_join_message_for_each_channel(
	std::list<Channel const *const> const &joined_channels,
	Client                                &sender)
{
	for (std::list<Channel const *const>::const_iterator channel = joined_channels.begin();
	     channel != joined_channels.end();
	     ++channel)
	{
		ChannelName const &chan_name = channel->get_name();

		sender.append_to_msg_out(sender.prefix() + "JOIN : Successfully joined " + chan_name);
		sender.append_to_msg_out(sender.formatted_reply(RPL_TOPIC, chan_name, channel->get_topic()));
		//names(sender, {chan_name});
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
	if (sender.has_mode(AlreadySentPass) == false)
		sender.append_to_msg_out(':' + this->_name + " Password required.\nTry /quote PASS <password>");
	if (params.size() < 1)
	{
		sender->append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "JOIN"));
		return;
	}
	if (params.size() == 1 && params[0] == '0')
	{
		for (std::map<ChannelName, Channel *const>::const_iterator it = sender->get_joined_channels_by_name().begin();
		     it != sender->get_joined_channels_by_name().end();
		     ++it)
		{
			std::vector<std::string> channel_and_msg;

			channel_and_msg.push_back(it->first);
			channel_and_msg.push_back(quit_msg);
			part(sender, channel_and_msg);
		}
	}

	std::vector<std::pair<std::string, std::string>> channel_key_pairs = split_channels_keys(params[0], params[1]);
	std::list<Channel const *const>                  joined_channels;

	for (size_t i = 0; i < channel_key_pairs.size(); ++i)
	{
		std::map<ChannelName, Channel *const>::const_iterator chan_by_name =
			this->_channels_by_name.find(channel_key_pairs[i].first);

		Channel const *const new_joined_channel =
			chan_by_name == this->_channels_by_name.end()
				? __join_new_channel(sender, ChannelName(channel_key_pairs[i].first))
				: __join_existing_channel(sender, chan_by_name->second, channel_key_pairs[i].second);

		if (new_joined_channel)
			joined_channels.push_back(new_joined_channel);
	}
	__send_join_message_for_each_channel(joined_channels, sender);
}
