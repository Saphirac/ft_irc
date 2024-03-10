/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 22:30:18 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

inline static void __set_topic(Client const &sender, NickName const &nickname, Channel &channel, std::string const &topic)
{
	if (channel.has_mode(RestrictedTopic) && !channel.has_operator(nickname))
		sender.append_to_msg_out(sender.formatted_reply(ERR_CHANOPRIVSNEEDED, nickname));
	else
	{
		channel.set_topic(topic);
		channel.broadcast_to_all_members("TOPIC : " + topic);
	}
}

inline static void __display_topic(Client &sender, Channel &channel)
{
	ChannelName const &chan_name = channel.get_name();
	
	if (topic.empty())
		sender.append_to_msg_out(sender.formatted_reply(RPL_NOTOPIC, chan_name));
	else
		sender.append_to_msg_out(sender.formatted_reply(RPL_TOPIC, chan_name, channel.get_topic()));
}

void Server::_topic(Client &sender, std::vector<std::string> const &params)
{
	if (params.empty())
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NEEDMOREPARAMS, "TOPIC"));
		return;
	}

	ChannelName const &chan_name = params[0];
	Channel const     &channel = this->_channels_by_name.find(chan_name);
	NickName const    &nickname = sender.get_nickname();

	if (channel == this->_channels_by_name.end() || !channel.has_member(nickname))
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NOTONCHANNEL, chan_name));
		return;
	}

	if (!params[1].empty() && params[1][0] == ':')
		__set_topic(nickname, channel, params[1].substr(1));
	else
		__display_topic(sender, channel);
}
