/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 03:22:04 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

typedef std::list<std::string> StringList;

inline static bool is_channel(std::string const &s) { return s[0] == '#' || s[0] == '&' || s[0] == '+' || s[0] == '!'; }

inline static void notice_to_channel(
	Client            &sender,
	Channel const     &channel,
	ChannelName const &chan_name,
	std::string const &msg)
{
	if (channel.get_modes().is_set(NoMessagesFromOutside) && !channel.has_member(sender))
		return;
	if (!msg.empty())
		channel.broadcast_to_all_members_but_one(sender.prefix() + "PRIVMSG " + chan_name + " :" + msg, sender);
}

inline static void notice_to_user(Client &sender, Client &receiver, std::string const &msg)
{
	if (!receiver.get_modes().is_set(Away))
		receiver.append_to_msg_out(sender.prefix() + "PRIVMSG " + receiver.get_nickname() + " :" + msg);
}

void Server::_notice(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser) || params.size() < 2)
		return;

	StringList list_of_targets = split<StringList>(params[0], ',');

	for (StringList::const_iterator target = list_of_targets.begin(); target != list_of_targets.end(); ++target)
	{
		if (is_channel(params[0]))
		{
			ChannelName const &chan_name = ChannelName(params[0]);

			if (this->_channels_by_name.count(chan_name) != 0)
				notice_to_channel(sender, this->_channels_by_name[chan_name], chan_name, params[1]);
		}
		else
		{
			NickName const &nickname = NickName(params[0]);

			if (this->_clients_by_nickname.count(nickname) != 0)
				notice_to_user(sender, *this->_clients_by_nickname[nickname], params[1]);
		}
	}
}
