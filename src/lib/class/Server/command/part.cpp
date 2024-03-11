/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 14:14:42 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

#define DEFAULT_PART_MESSAGE "Goodbye to all!"

typedef std::list<ChannelName>          ChannelNameList;
typedef ChannelNameList::const_iterator ChannelNameIterator;

/**
 * @brief Remove member of each channel sent in params
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw std:exception if a std function critically fails
 */
void Server::_part(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PART");

	ChannelNameList const     channel_names = split<ChannelNameList>(parameters[0], ',');
	ChannelNameIterator const end = channel_names.end();

	for (ChannelNameIterator channel_name = channel_names.begin(); channel_name != end; ++channel_name)
	{
		std::map<ChannelName, Channel>::iterator const channel_by_name = this->_channels_by_name.find(*channel_name);

		if (channel_by_name == this->_channels_by_name.end())
			return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &*channel_name);

		Channel &channel = channel_by_name->second;

		if (!channel_by_name->second.has_member(sender))
			return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &*channel_name);

		channel.broadcast_to_all_members(
			sender.prefix() + "PART " + *channel_name + ' '
			+ (parameters.size() > 1 ? parameters[1] : DEFAULT_PART_MESSAGE));
		channel.remove_member(sender);
	}
}
