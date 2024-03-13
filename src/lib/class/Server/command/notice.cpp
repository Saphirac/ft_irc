/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/13 09:46:46 by jodufour         ###   ########.fr       */
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
inline static void notice_to_channel(
	Client            &sender,
	ChannelName const &target_name,
	Channel const     &target,
	std::string const &msg)
{
	if (target.get_modes().is_set(NoMessagesFromOutside) && !target.has_member(sender))
		return;

	target.broadcast_to_all_members_but_one(sender.prefix() + "NOTICE " + target_name + " :" + msg, sender);
}

// TODO: Write the doxygen comment of this function
inline static void notice_to_user(
	Client            &sender,
	NickName const    &target_nickname,
	Client            &target,
	std::string const &msg)
{
	Client::Modes const &target_modes = target.get_modes();

	if (target_modes.is_set(Away))
		return;

	target.append_to_msg_out(sender.prefix() + "NOTICE " + target_nickname + " :" + msg);
}

void Server::_notice(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.is_registered() || parameters.size() < 2)
		return;

	StringList const targets = split<StringList>(parameters[0], ',');

	for (StringList::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (is_channel(*target))
		{
			ChannelName const               &channel_name = ChannelName(*target);
			ChannelMap::const_iterator const channel_by_name = this->_channels_by_name.find(channel_name);

			if (channel_by_name == this->_channels_by_name.end())
				return;

			notice_to_channel(sender, channel_name, channel_by_name->second, parameters[1]);
		}
		else
		{
			NickName const                                   &nickname = NickName(*target);
			std::map<NickName, Client *const>::const_iterator client_by_nickname =
				this->_clients_by_nickname.find(nickname);

			if (client_by_nickname == this->_clients_by_nickname.end())
				return;

			notice_to_user(sender, nickname, *client_by_nickname->second, parameters[1]);
		}
	}
}
