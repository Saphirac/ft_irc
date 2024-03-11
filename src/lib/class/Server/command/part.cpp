/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 08:27:42 by mcourtoi         ###   ########.fr       */
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
void Server::_part(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(':' + this->_name + " You are not registered.\n");
	if (params.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PART");

	std::list<std::string> channels = split<std::list<std::string> >(params[0], ',');

	for (std::list<std::string>::const_iterator actual_channel = channels.begin(); actual_channel != channels.end();
	     ++actual_channel)
	{
		ChannelName const                       &chan_name = *actual_channel;
		std::map<ChannelName, Channel>::iterator chan_by_name = this->_channels_by_name.find(chan_name);

		if (chan_by_name == this->_channels_by_name.end())
			return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &chan_name);

		Channel &channel = chan_by_name->second;

		if (!chan_by_name->second.has_member(sender))
			return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &chan_name);

		channel.remove_member(sender);
		if (params.size() > 1)
		{
			sender.append_to_msg_out(sender.prefix() + "PART " + params[1]);
			channel.broadcast_to_all_members(sender.prefix() + "PART " + params[1]);
		}
		else
		{
			sender.append_to_msg_out(sender.prefix() + "PART " + chan_name + " " + DEFAULT_PART_MESSAGE);
			channel.broadcast_to_all_members(sender.prefix() + "PART " + DEFAULT_PART_MESSAGE);
		}
	}
}
