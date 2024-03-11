/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 15:03:43 by mcourtoi         ###   ########.fr       */
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
		return ;
	channel.broadcast_to_all_members(sender.prefix() + "NOTICE " + chan_name + " :" + msg);
}

inline static void notice_to_user(Client &sender, NickName const &nickname, Client &receiver, std::string const &msg)
{
	Client::Modes const &user_modes = receiver.get_modes();

	if (!user_modes.is_set(Away))
		receiver.append_to_msg_out(sender.prefix() + "NOTICE " + nickname + " :" + msg);
}

void Server::_notice(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser) || params.size() < 2)
		return ;

	StringList list_of_targets = split<StringList>(params[0], ',');

	for (StringList::const_iterator target_iterator = list_of_targets.begin(); target_iterator != list_of_targets.end(); ++target_iterator)
	{
		std::string const &target = *target_iterator;

		if (is_channel(target))
		{
			ChannelName const &chan_name = ChannelName(target);

			if (this->_channels_by_name.count(chan_name))
				notice_to_channel(sender, this->_channels_by_name[chan_name], chan_name, params[1]);
		}
		else
		{
			NickName const &nickname = NickName(target);

			if (this->_clients_by_nickname.count(nickname))
				notice_to_user(sender, nickname, *this->_clients_by_nickname[nickname], params[1]);
		}
	}
}

