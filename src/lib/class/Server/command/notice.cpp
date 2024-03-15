/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:28:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 12:40:37 by jodufour         ###   ########.fr       */
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
	bool const sender_is_member = target.has_member(sender);

	if (target.get_modes().is_set(NoMessagesFromOutside) && sender_is_member)
		return;

	sender_is_member
		? target.broadcast_to_all_members_but_one(sender.prefix() + "NOTICE " + target_name + " :" + msg, sender)
		: target.broadcast_to_all_members(sender.prefix() + "NOTICE " + target_name + " :" + msg);
}

void Server::_notice(Client &sender, CommandParameterVector const &parameters)
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
				continue;

			Channel const        &channel = channel_by_name->second;
			Channel::Modes const &channel_modes = channel.get_modes();

			if (channel_modes.has_ban_mask(sender.get_nickname())
			    || (channel_modes.is_set(NoMessagesFromOutside) && !channel.has_member(sender)))
				continue;

			notice_to_channel(sender, channel_name, channel, parameters[1]);
		}
		else
		{
			NickName const           &nickname = NickName(*target);
			ClientMap::const_iterator user_by_nickname = this->_clients_by_nickname.find(nickname);

			if (user_by_nickname == this->_clients_by_nickname.end())
				return;

			(*user_by_nickname->second)
				.append_to_msg_out(sender.prefix() + "NOTICE " + nickname + " :" + parameters[1]);
		}
	}
}
