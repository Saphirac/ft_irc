/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/13 13:04:33 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

typedef std::list<ChannelName> ChannelNameList;

/**
 * @brief Gets information about either specific channels, or all the channels on the server.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_list(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);

	if (parameters.empty())
	{
		for (ChannelMap::const_iterator channel_by_name = this->_channels_by_name.begin();
		     channel_by_name != this->_channels_by_name.end();
		     ++channel_by_name)
		{
			ChannelName const &channel_name = channel_by_name->first;
			Channel const     &channel = channel_by_name->second;

			sender.append_formatted_reply_to_msg_out(
				RPL_LIST,
				&channel_name,
				static_cast<uint8_t>(channel.member_count()),
				&channel.get_topic());
		}
	}
	else
	{
		ChannelNameList const channel_names = split<ChannelNameList>(parameters[0], ',');

		for (ChannelNameList::const_iterator channel_name; channel_name != channel_names.end(); ++channel_name)
		{
			ChannelMap::const_iterator const channel_by_name = this->_channels_by_name.find(*channel_name);

			if (channel_by_name == this->_channels_by_name.end())
				continue;

			Channel const &channel = channel_by_name->second;

			sender.append_formatted_reply_to_msg_out(
				RPL_LIST,
				&channel_name,
				static_cast<uint8_t>(channel.member_count()),
				&channel.get_topic());
		}
	}
	sender.append_formatted_reply_to_msg_out(RPL_LISTEND);
}
