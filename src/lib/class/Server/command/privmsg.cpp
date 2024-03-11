/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 10:06:43 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

#ifdef DEBUG
#	include <iostream>
#endif

typedef std::list<std::string> StringList;

inline static bool is_channel(std::string const &s) { return s[0] == '#' || s[0] == '&' || s[0] == '+' || s[0] == '!'; }

inline static void privmsg_to_channel(
	Client            &sender,
	ChannelName const &chan_name,
	Channel const     &channel,
	std::string const &msg)
{

#ifdef DEBUG
	std::cerr << "privmsg_to_channel " << chan_name << " " << msg << std::endl;
#endif

	if (channel.get_modes().is_set(NoMessagesFromOutside) && !channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &chan_name);
	channel.broadcast_to_all_members(sender.prefix() + "PRIVMSG " + chan_name + ' ' + msg);
}

inline static void privmsg_to_user(Client &sender, NickName const &nickname, Client &receiver, std::string const &msg)
{
	Client::Modes const &user_modes = receiver.get_modes();

	if (user_modes.is_set(Away))
		sender.append_formatted_reply_to_msg_out(RPL_AWAY, &nickname, &user_modes.get_away_msg());
	receiver.append_to_msg_out(sender.prefix() + "PRIVMSG " + nickname + ' ' + msg);
}

void Server::_privmsg(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(sender.prefix() + "You are not registered.");
	if (params.size() < 2)
	{
		std::string const &to_check = params[0].substr(0, ',');

		if (NickName(to_check).is_valid() || ChannelName(to_check).is_valid())
			return sender.append_formatted_reply_to_msg_out(ERR_NOTEXTTOSEND, "PRIVMSG");
		return sender.append_formatted_reply_to_msg_out(ERR_NORECIPIENT, "PRIVMSG");
	}

#ifdef DEBUG
	std::cerr << "PRIVMSG " << params[0] << " " << params[1] << std::endl;
#endif

	StringList list_of_targets = split<StringList>(params[0], ',');

	for (StringList::const_iterator target_iterator = list_of_targets.begin(); target_iterator != list_of_targets.end(); ++target_iterator)
	{
		std::string const &target = *target_iterator;

		if (is_channel(target))
		{
			ChannelName const &chan_name = ChannelName(target);

			if (this->_channels_by_name.count(chan_name) == 0)
				sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &target);
			else
				privmsg_to_channel(sender, chan_name, this->_channels_by_name[chan_name], params[1].substr(1));
		}
		else
		{
			NickName const &nickname = NickName(target);

			if (this->_clients_by_nickname.count(nickname) == 0)
				sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &target);
			else
				privmsg_to_user(sender, nickname, *this->_clients_by_nickname[nickname], params[1].substr(1));
		}
	}
}
