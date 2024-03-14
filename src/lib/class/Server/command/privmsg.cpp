/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/14 23:24:47 by jodufour         ###   ########.fr       */
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
inline static void privmsg_to_channel(
	Client            &sender,
	ChannelName const &target_name,
	Channel const     &target,
	std::string const &msg)
{
	if (target.get_modes().is_set(NoMessagesFromOutside) && !target.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &target_name);

	target.broadcast_to_all_members_but_one(sender.prefix() + "PRIVMSG " + target_name + " :" + msg, sender);
}

// TODO: Write the doxygen comment of this function
inline static void privmsg_to_user(
	Client            &sender,
	NickName const    &target_nickname,
	Client            &target,
	std::string const &msg)
{
	Client::Modes const &target_modes = target.get_modes();

	if (target_modes.is_set(Away))
		sender.append_formatted_reply_to_msg_out(RPL_AWAY, &target_nickname, &target_modes.get_away_msg());

	target.append_to_msg_out(sender.prefix() + "PRIVMSG " + target_nickname + " :" + msg);
}

// TODO: Write the doxygen comment of this method
void Server::_privmsg(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NORECIPIENT, "PRIVMSG");
	if (parameters.size() < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NOTEXTTOSEND, "PRIVMSG");

	StringList const targets = split<StringList>(parameters[0], ',');

	for (StringList::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (is_channel(*target))
		{
			ChannelName const               &channel_name = ChannelName(*target);
			ChannelMap::const_iterator const channel_by_name = this->_channels_by_name.find(channel_name);

			if (channel_by_name == this->_channels_by_name.end())
			{
				sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &channel_name);
				continue;
			}

			Channel const        &channel = channel_by_name->second;
			Channel::Modes const &channel_modes = channel.get_modes();

			if (channel_modes.has_ban_mask(sender.get_nickname())
			    || (channel_modes.is_set(NoMessagesFromOutside) && !channel.has_member(sender)))
			{
				sender.append_formatted_reply_to_msg_out(ERR_CANNOTSENDTOCHAN, &channel_name);
				continue;
			}

			privmsg_to_channel(sender, channel_name, channel_by_name->second, parameters[1]);
		}
		else
		{
			NickName const           &nickname = NickName(*target);
			ClientMap::const_iterator client_by_nickname = this->_clients_by_nickname.find(nickname);

			if (client_by_nickname == this->_clients_by_nickname.end())
			{
				sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &nickname);
				continue;
			}

			privmsg_to_user(sender, nickname, *client_by_nickname->second, parameters[1]);
		}
	}
}
