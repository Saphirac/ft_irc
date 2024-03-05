/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 20:16:19 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

inline static void	__set_topic(Nickname &sender, Channel &channel, std::string const &topic)
{
	if (channel.has_mode(TopicProtected) && !channel.has_operator(sender))
		sender.append_to_msg_out(format_reply(ERR_CHANOPRIVSNEEDED, sender));
	else
	{
		channel.set_topic(topic);
		channel.broadcast_to_all_members("TOPIC : " + topic);
	}
}

inline static void __display_topic(Client &sender, Channel &channel)
{
	std::string const &topic = channel.get_topic();
	char const *chan_name_str = channel.get_name().c_str();
	if (topic.empty())
		sender.append_to_msg_out(format_reply(RPL_NOTOPIC, chan_name_str));
	else
		sender.append_to_msg_out(format_reply(RPL_TOPIC, chan_name_str, topic));
}

void	Server::topic(Client &sender, std::vector<std::string> const &params)
{
	if (params.empty())
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "TOPIC"));
		return ;
	}
	
	ChannelName const &chan_name = params[0];
	Channel const &channel = this->_channels_by_name.find(chan_name);
	Nickname const &nickname = sender.get_nickname();
	
	if (channel == this->_channels_by_name.end() || !channel.has_member(nickname))
	{
		sender.append_to_msg_out(format_reply(ERR_NOTONCHANNEL, chan_name.c_str()));
		return ;
	}
	
	if (!params[1].empty() && params[1][0] == ':')
		__set_topic(nickname, channel, params[1].substr(1));
	else
		__display_topic(sender, channel);
}
