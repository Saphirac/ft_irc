/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 05:29:01 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

inline static void set_topic(Client &sender, Channel &channel, ChannelName const &chan_name, Topic const &topic)
{
	Channel::Modes const &modes = channel.get_modes();

	if (modes.is_set(RestrictedTopic) && !modes.has_operator(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &sender.get_nickname());

	channel.set_topic(topic.is_valid() ? topic : Topic());
	channel.broadcast_to_all_members(sender.prefix() + "TOPIC " + chan_name + " :" + channel.get_topic());
}

inline static void send_back_current_topic(Client &sender, Channel const &channel, ChannelName const &chan_name)
{
	Topic const &topic = channel.get_topic();

	if (topic.empty())
		return sender.append_formatted_reply_to_msg_out(RPL_NOTOPIC, &chan_name);

	sender.append_formatted_reply_to_msg_out(RPL_TOPIC, &chan_name, &topic);
}

void Server::_topic(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "TOPIC");

	ChannelName const    &channel_name = parameters[0];
	ChannelIterator const channel_by_name = this->_channels_by_name.find(channel_name);

	if (channel_by_name == this->_channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_name);

	Channel &channel = channel_by_name->second;

	if (!channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &channel_name);

	if (parameters.size() > 1)
		return set_topic(sender, channel, channel_name, Topic(parameters[1]));
	send_back_current_topic(sender, channel, channel_name);
}
