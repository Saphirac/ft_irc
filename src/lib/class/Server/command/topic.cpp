/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 09:56:48 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

inline static void __set_topic(Client &sender, Channel &channel, ChannelName const &chan_name, Topic const &topic)
{
	Channel::Modes const &modes = channel.get_modes();

	if (modes.is_set(RestrictedTopic) && !modes.has_operator(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &sender.get_nickname());

	channel.set_topic(topic.is_valid() ? topic : Topic());
	channel.broadcast_to_all_members(sender.prefix() + "TOPIC " + chan_name + " :" + channel.get_topic());
}

inline static void __display_topic(Client &sender, Channel const &channel, ChannelName const &chan_name)
{
	Topic const &topic = channel.get_topic();

	if (topic.empty())
		return sender.append_formatted_reply_to_msg_out(RPL_NOTOPIC, &chan_name);

	sender.append_formatted_reply_to_msg_out(RPL_TOPIC, &chan_name, &topic);
}

void Server::_topic(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(sender.prefix() + " You are not registered.\n");
	if (params.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "TOPIC");

	ChannelName const &chan_name = params[0];
	// REMIND: change the type to ChannelIterator
	std::map<ChannelName, Channel>::iterator channel_by_name = this->_channels_by_name.find(chan_name);

	if (channel_by_name == this->_channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &chan_name);

	Channel &channel = channel_by_name->second;

	if (!channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &chan_name);

	// REMIND: remove the substr(1)

	if (params.size() > 1)
		return __set_topic(sender, channel, chan_name, Topic(params[1].substr(1)));
	__display_topic(sender, channel, chan_name);
}
