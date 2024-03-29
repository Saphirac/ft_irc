/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 00:26:52 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

typedef std::list<ChannelName> ChannelNameList;

/**
 * @brief Remove member of each channel sent in params
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw std:exception if a std function critically fails
 */
void Server::_part(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PART");

	ChannelNameList const channel_names = split<ChannelNameList>(parameters[0], ',');

	for (ChannelNameList::const_iterator channel_name = channel_names.begin(); channel_name != channel_names.end();
	     ++channel_name)
	{
		ChannelMap::iterator const channel_by_name = this->_channels_by_name.find(*channel_name);

		if (channel_by_name == this->_channels_by_name.end())
			return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &*channel_name);
		if (!channel_by_name->second.has_member(sender))
			return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &*channel_name);

		make_user_leave_channel(
			this->_channels_by_name,
			sender,
			*channel_name,
			sender.prefix() + "PART " + *channel_name + ' '
				+ (parameters.size() > 1 ? parameters[1] : DEFAULT_PART_TEXT));
	}
}
