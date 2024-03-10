/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 22:56:44 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"

inline static void __kick_client(Channel const &channel, std::string const &user_to_remove_nickname)
{
	Client            &client = this->_clients_by_nickname[user_to_remove_nickname];
	ChannelName const &channel_name = channel.get_name();

	channel.remove_member(client);
	client.remove_channel(channel);
	client.append_to_msg_out(client.prefix() + "KICK :" + channel_name + " " + user_to_remove_nickname);
	channel.broadcast_to_all_members(client.prefix() + "KICK " + channel_name + " " + user_to_remove_nickname);
}

inline static void __kick_only_one_channel_given(
	Client const            &client,
	std::vector<std::string> split_channels_names,
	std::vector<std::string> split_nicknames)
{
	Channel const     &channel = this->_channels_by_name[split_channels_names[0]];
	ChannelName const &chan_name = channel.get_name();

	if (this->_channels_by_name.count(split_channels_names[0]))
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NOSUCHCHANNEL, split_channels_names[0]));
		return;
	}
	if (channel.has_operator(sender) == false)
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_CHANOPRIVSNEEDED, chan_name));
		return;
	}
	for (size_t i = 0; !split_nicknames[i].empty(); ++i)
	{
		if (channel.has_member(this->_client_by_nickname[split_nicknames[i]]) == false)
			sender.append_to_msg_out(sender.formatted_reply(ERR_USERNOTONCHANNEL, split_nicknames[i], chan_name));
		else
			__kick_client(channel, split_nicknames[i]);
	}
}

inline static void __kick_multiple_channel_given(
	Client const            &sender,
	std::vector<std::string> split_channels_names,
	std::vector<std::string> split_nicknames,
	size_t split_channels_names_size)
{
	for (size_t i = 0; i < split_channels_names_size; ++i)
	{
		if (this->_channels_by_name.count(split_channels_names[i]) == 0)
			sender.append_to_msg_out(sender.formatted_reply(ERR_NOSUCHCHANNEL, split_channels_names[i]));
		else
		{
			Channel const     &channel = this->_channels_by_name[split_channels_names[i]];
			ChannelName const &chan_name = channel.get_name();

			if (!channel.has_operator(sender))
				sender.append_to_msg_out(sender.formatted_reply(ERR_CHANOPRIVSNEEDED, chan_name));
			else if (!channel.has_member(this->_client_by_nickname[split_nicknames[i]]))
				sender.append_to_msg_out(sender.formatted_reply(ERR_USERNOTONCHANNEL, split_nicknames[i], chan_name));
			else
				__kick_client(channel, split_nicknames[i], chan_name);
		}
	}
}

void Server::_kick(Client &sender, std::vector<std::string> const &params)
{
	if (params.size() < 2)
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NEEDMOREPARAMS, "KICK"));
		return;
	}

	std::vector<std::string> split_channels_names = split<std::vector<std::string> >(params[0], ',');
	std::vector<std::string> split_nicknames = split<std::vector<std::string> >(params[1], ',');

	size_t split_channels_names_size = split_channels_names.size();

	if (split_channels_names_size != 1 && split_nicknames.size() != split_channels_names_size)
	{
		sender.append_to_msg_out(sender.formatted_reply(ERR_NEEDMOREPARAMS, "KICK"));
		return;
	}

	if (split_channels_names_size == 1)
		__kick_only_one_channel_given(sender, split_channels_names, split_nicknames);
	else
		__kick_multiple_channel_given(sender, split_channels_names, split_nicknames);

}
