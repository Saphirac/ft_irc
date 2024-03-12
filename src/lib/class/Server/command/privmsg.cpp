/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 22:34:35 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

typedef std::list<std::string> StringList;

static std::string const possible_channel_prefix = "#&+!";

// TODO: Write the doxygen comment of this function
inline static bool is_channel(std::string const &s) { return possible_channel_prefix.find(s[0]) != std::string::npos; }

// TODO: Write the doxygen comment of this function
// TODO : create a broadcast_to_all_members except the sender
inline static void privmsg_to_channel(
	Client            &sender,
	Channel const     &channel,
	ChannelName const &chan_name,
	std::string const &msg)
{
	if (channel.get_modes().is_set(NoMessagesFromOutside) && !channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &chan_name);
	channel.broadcast_to_all_members(sender.prefix() + "PRIVMSG " + chan_name + " :" + msg);
}

// TODO: Write the doxygen comment of this function
inline static void privmsg_to_user(Client &sender, NickName const &nickname, Client &receiver, std::string const &msg)
{
	Client::Modes const &user_modes = receiver.get_modes();

	if (user_modes.is_set(Away))
		sender.append_formatted_reply_to_msg_out(RPL_AWAY, &nickname, &user_modes.get_away_msg());
	receiver.append_to_msg_out(sender.prefix() + "PRIVMSG " + nickname + " :" + msg);
}

// TODO: Write the doxygen comment of this method
void Server::_privmsg(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NORECIPIENT, "PRIVMSG");
	if (parameters.size() < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NOTEXTTOSEND, "PRIVMSG");

	StringList const                 targets = split<StringList>(parameters[0], ',');

	for (StringList::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (is_channel(*target))
		{
			ChannelName const         &channel_name = ChannelName(*target);
			ChannelConstIterator const channel_by_name = this->_channels_by_name.find(channel_name);

			if (channel_by_name == this->_channels_by_name.end())
				sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &channel_name);
			else
				privmsg_to_channel(sender, channel_by_name->second, channel_name, parameters[1]);
		}
		else
		{
			NickName const                                   &nickname = NickName(*target);
			std::map<NickName, Client *const>::const_iterator client_by_nickname =
				this->_clients_by_nickname.find(nickname);

			if (client_by_nickname == this->_clients_by_nickname.end())
				sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &nickname);
			else
				privmsg_to_user(sender, nickname, *client_by_nickname->second, parameters[1]);
		}
	}
}
