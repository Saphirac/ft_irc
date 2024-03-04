/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 19:28:30 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include <iostream>

inline static Channel const *__join_new_channel(Client &sender, ChannelName const &chan_name)
{
	if (sender->get_channels.size() > MAX_CHANNELS)
		sender->append_to_msg_out(format_reply(ERR_TOOMANYCHANNELS));
	else if (chan_name.is_valid())
	{
		Channel &channel = this->_channels_by_name[chan_name];
		sender->add_channel(channel);
		sender.append_to_msg_out(format_reply(RPL_TOPIC))
	}
	else
		sender->append_to_msg_out(format_reply(ERR_NOSUCHCHANNEL, chan_name.c_str()));
}

inline static Channel const *__join_existing_channel(Client &sender, Channel &channel, std::string const &key)
{
	std::string const &nickname = sender.get_nickname();
	if (channel->has_member(nickname) == false)
	{
		Channel::Modes const &channel_modes = channel->get_modes();
		size_t                user_limit = channel_modes->get_limit();
		if (user_limit != 0 || user_limit >= channel->get_members().size())
			sender->append_to_msg_out(format_reply(ERR_CHANNELISFULL, channel->get_name().c_str()));
		else if (channel->has_mode(InviteOnly) && !channel_modes.has_invite_mask(nickname))
			sender->append_to_msg_out(format_reply(ERR_INVITEONLYCHAN, channel->get_name().c_str()));
		else if (sender.get_channels.size() > MAX_CHANNELS)
			sender->append_to_msg_out(format_reply(ERR_TOOMANYCHANNELS));
		else if (channel_modes.has_ban_mask(nickname))
			sender->append_to_msg_out(format_reply(ERR_BANNEDFROMCHAN, channel->get_name().c_str()));
		else if (channel->has_mode(Key) && channel_modes.get_key() != key)
			sender->append_to_msg_out(format_reply(ERR_BADCHANNELKEY, channel->get_name().c_str()));
		else
		{
			channel->add_member(nickname);
			sender->add_channel(channel);
			return channel;
		}
	}
	return NULL;
}

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

// TODO : Change the password verification up ahead before executing any command

void Server::join(Client &sender, std::vector<std::string> const &params)
{
	if (sender.has_mode(AlreadySentPass) == false)
		sender.append_to_msg_out(this->_name + " Password required.\nTry /quote PASS <password>");

	if (params.size() == 1 && params[0] == '0')
		sender->part_all_channels("Quitting all channels.");

	std::vector<std::pair<std::string, std::string>>      channel_key_pairs = split_channels_keys(params[0], params[1]);
	std::map<ChannelName, Channel *const>::const_iterator chan_by_name = this->_channels_by_name.find(params[i]);
	std::list<Channel const *const>                       joined_channels;

	for (size_t i = 0; i < channel_key_pairs.size(); ++i)
	{
		Channel const *const new_joined_channel =
			chan_by_name == this->_channels_by_name.end()
				? __join_new_channel(sender, ChannelName(channel_key_pairs[i].first))
				: __join_existing_channel(sender, chan_by_name->second, channel_key_pairs[i].second);

		if (new_joined_channel)
			joined_channels.push_back(new_channel);
	}
}
