/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 14:45:33 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <iostream>

// TODO : Change the password verification up ahead before executing any command

void Server::join(Client &sender, std::vector<std::string> const &params)
{
	if (sender.has_mode(AlreadySentPass) == false)
		sender.append_to_msg_out(this->_name + " Password required.\nTry /quote PASS <password>");

	if (params.size() == 1 && params[0] == '0')
		sender->part_all_channels("Quitting all channels.");

	if (i == 0)
	{
		std::map<ChannelName, Channel *const>::const_iterator chan_by_name = this->_channels_by_name.find(params[i]);
		if (chan_by_name == this->_channels.end())
		{
			ChannelName chan_name(params[i]);
			if (chan_name.is_valid())
			{
				this->_channels[chan_name] = &Channel();
				sender->add_channel(this->_channels[chan_name]);
			}
			else
				sender->append_to_msg_out(format_reply(ErrNoSuchChannel, chan_name.c_str()));
		}
		else
		{
			Channel const     &channel = chan->second;
			std::string const &nickname = sender.get_nickname();
			if (channel->has_member(nickname) == false)
			{
				user_limit = channel->get_user_limit();
				if (user_limit() != 0 || user_limit() >= channel->get_members().size())
					sender->append_to_msg_out(format_reply(ErrChannelIsFull, channel->get_name().c_str()));
				else
				{
					Channel::Modes const &channel_modes = channel->get_modes();
					if (channel->has_mode(InviteOnly) && channel_modes.has_invite_mask(nickname))
					{
						channel->add_member(nickname);
						sender->add_channel(channel);
					}
					sender->add_channel(channel);
				}
			}
		}
	}
	for (size_t i = 1; !params[i].empty() && params[i][0] == ','; ++i)
	{
		ChannelName                                           chan_name(params[i].substr(1, std::string::npos));
		std::map<ChannelName, Channel *const>::const_iterator chan_by_name = this->_channels_by_name.find(chan_name);
		if (chan_by_name == this->_channels.end())
		{
			if (chan_name.is_valid())
			{
				this->_channels[chan_name] = &Channel();
				Channel const &channel = this->_channels[chan_name];
				sender->add_channel(channel);
				channel->add_member(sender.get_nickname());
			}
			else
				sender->append_to_msg_out(format_reply(ErrNoSuchChannel, chan_name.c_str()));
		}
	}
}
