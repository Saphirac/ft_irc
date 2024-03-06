/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/06 03:26:21 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

// TODO : make the comment, separate into 2 more functions
void Server::kick(Client &sender, std::vector<std::string> const &params)
{
	if (params.size() < 2)
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "KICK"));
		return;
	}
	std::vector<std::string> split_channels_names = split<std::vector<std::string> >(params[0], ',');
	std::vector<std::string> split_nicks = split<std::vector<std::string> >(params[1], ',');

	size_t split_channels_names_size = split_channels_names.size();

	if (split_channels_names_size != 1 && split_nicks.size() != split_channels_names_size)
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "KICK"));
		return;
	}
	if (split_channels_names_size == 1)
	{
		if (this->_channels_by_name.count(split_channels_names[0]))
		{
			sender.append_to_msg_out(format_reply(ERR_NOSUCHCHANNEL, split_channels_names[0]));
			return;
		}

		Channel const &channel = this->_channels_by_name[split_channels_names[0]];

		if (channel.has_operator(sender.get_nickname()) == false)
		{
			sender.append_to_msg_out(format_reply(ERR_CHANOPRIVSNEEDED, channel.get_name().c_str()));
			return;
		}
		for (size_t i = 0; !split_nick[i].empty(); ++i)
		{
			if (channel.has_member(split_nicks[i]) == false)
				sender.append_to_msg_out(format_reply(ERR_USERNOTONCHANNEL, split_nicks[i], channel.get_name()));
			else
			{
				Client &client = this->_clients_by_nickname[split_nicks[i]];
				Nickname const &nickname = client.get_nickname();

				channel.remove_member(client);
				client.remove_channel(channel);
				client.append_to_msg_out("KICK :" + channel.get_name() + " " + nickname);
				channel.broadcast_to_all_members("KICK " + channel.get_name() + " " + nickname);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < split_channels_names_size; ++i)
		{
			if (this->_channels_by_name.count(split_channels_names[i]) == 0)
				sender.append_to_msg_out(format_reply(ERR_NOSUCHCHANNEL, split_channels_names[i]));
			else
			{
				Channel const &channel = this->_channels_by_name[split_channels_names[i]];
				ChannelName const &chan_name = channel.get_name();
				
				if (!channel.has_operator(sender.get_nickname()))
					sender.append_to_msg_out(format_reply(ERR_CHANOPRIVSNEEDED, chan_name));
				else if (!channel.has_member(split_nicks[i]))
					sender.append_to_msg_out(format_reply(ERR_USERNOTONCHANNEL, split_nicks[i], chan_name));
				else
				{
					Client &client = this->_clients_by_nickname[split_nicks[i]];
					Nickname const &nickname = client.get_nickname();
					
					channel.remove_member(client);
					client.remove_channel(channel);
					client.append_to_msg_out("KICK :" + chan_name + " " + nickname);
					channel.broadcast_to_all_members("KICK " + chan_name + " " + nickname);
				}
			}
		}
	}
}
