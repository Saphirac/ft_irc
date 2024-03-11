/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 07:30:28 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

inline static bool is_channel(std::string const &s) { return s[0] == '#' || s[0] == '&' || s[0] == '+' || s[0] == '!'; }

inline static void privmsg_to_channel(
	Client            &sender,
	ChannelName const &chan_name,
	Channel const     &channel,
	std::string const &msg)
{
	if (channel.get_modes().is_set(NoMessagesFromOutside) && !channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &chan_name);
	channel.broadcast_to_all_members(sender.prefix() + "PRIVMSG " + msg);
}

inline static void privmsg_to_user(Client &sender, NickName const &nickname, Client &receiver, std::string const &msg)
{
	Client::Modes const &user_modes = receiver.get_modes();

	if (user_modes.is_set(Away))
		sender.append_formatted_reply_to_msg_out(RPL_AWAY, &nickname, &user_modes.get_away_msg());
	receiver.append_to_msg_out(sender.prefix() + "PRIVMSG " + msg);
}

void Server::_privmsg(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(':' + this->_name + " You are not registered.\n");
	if (params.size() < 2)
	{
		std::string const &to_check = params[0].substr(0, ',');

		if (NickName(to_check).is_valid() || ChannelName(to_check).is_valid())
			return sender.append_formatted_reply_to_msg_out(ERR_NOTEXTTOSEND, "PRIVMSG");
		return sender.append_formatted_reply_to_msg_out(ERR_NORECIPIENT, "PRIVMSG");
	}

	std::list<std::string> list_of_targets = split<std::list<std::string> >(params[0], ',');

	for (std::list<std::string>::const_iterator target = list_of_targets.begin(); target != list_of_targets.end();
	     ++target)
	{
		if (is_channel(params[0]))
		{
			ChannelName const &chan_name = ChannelName(params[0]);

			if (this->_channels_by_name.count(chan_name) == 0)
				sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &params[0]);
			else
				privmsg_to_channel(sender, chan_name, this->_channels_by_name[chan_name], params[1]);
		}
		else
		{
			NickName const &nickname = NickName(params[0]);

			if (this->_clients_by_nickname.count(nickname) == 0)
				sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &params[0]);
			else
				privmsg_to_user(sender, nickname, *this->_clients_by_nickname[nickname], params[1]);
		}
	}
}
