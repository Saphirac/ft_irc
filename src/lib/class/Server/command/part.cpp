/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 22:57:02 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

#define DEFAULT_PART_MESSAGE "Goodbye to all!"

/**
 * @brief Remove member of each channel sent in params
 *
 * @param sender the client asking to part
 * @param params the channel to part and the part-message
 *
 * @throw std:exception if a std function critically fails
 */
void Server::_part(Client &sender, std::vector<std::string> const &parameters)
{
	if (params.empty())
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NEEDMOREPARAMS, "PART"));
		return;
	}

	std::list<std::string> channels = split<std::list>(params[0], ',');

	for (std::list<std::string>::const_iterator actual_channel = channels.begin(); actual_channel != channels.end();
	     ++actual_channel)
	{
		ChannelName const                               &chan_name = actual_channel.get_name();
		std::map<ChannelName, Channel *>::const_iterator chan_by_name = this->_channels_by_name.find(chan_name);
		if (chan_by_name == this->_channels_by_name.end())
			sender.append_to_msg_out(sender.formatted_reply(ERR_NOSUCHCHANNEL, chan_name));
		else if (!chan_by_name->has_member(sender.get_nickname()))
			sender.append_to_msg_out(sender.formatted_reply(ERR_NOTONCHANNEL, chan_name));
		else
		{
			actual_channel->remove_member(sender);
			if (!params[1].empty())
			{
				sender.append_to_msg_out(sender.prefix() + "PART :" + params[1]);
				actual_channel->broadcast_to_all_members(sender.prefix() + "PART : " + params[1]);
			}
			else
			{
				sender.append_to_msg_out(sender.prefix() + "PART : Successfully left channel " + chan_name);
				actual_channel->broadcast_to_all_members(sender.prefix() + "PART : " + DEFAULT_PART_MESSAGE);
			}
		}
	}
}
