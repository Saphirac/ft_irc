/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 20:53:16 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Invite a user to a channel, adding him to the Invite list
 *
 * @param sender the client inviting
 * @param params the command parameters (nickname, channel)
 */
void Server::invite(Client &sender, std::vector<std::string> const &params)
{
	if (params.size() < 2)
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "INVITE"));
		return;
	}

	Nickname const &nickname = sender.get_nickname();
	Client const   &user_to_invite = this->_clients_by_nickname.find(params[0]);

	if (user_to_invite == this->_clients_by_nickname.end())
	{
		sender.append_to_msg_out(format_reply(ERR_NOSUCHNICK, params[0]));
		return;
	}

	Channel const &chan = this->_channels_by_name.find(params[1]);

	if (chan == this->_channels_by_name.end() || !chan.has_member(nickname))
		sender.append_to_msg_out(format_reply(ERR_NOTONCHANNEL, params[1]));
	else if (chan.has_member(params[0]))
		sender.append_to_msg_out(format_reply(ERR_USERONCHANNEL, params[0], params[1]));
	else if (chan.has_mode(InviteOnly) && !chan.has_operator(nickname))
		sender.append_to_msg_out(format_reply(ERR_CHANOPRIVSNEEDED, nickname));
	else
	{
		if (user_to_invite.is_away())
			sender.append_to_msg_out(format_reply(
				RPL_AWAY,
				params[0],
				user_to_invite.get_away_message()));
		else
		{
			chan.add_to_invite_list(user_to_invite);
			user_to_invite.append_to_msg_out("INVITE : " + nickname + " has invited you to " + params[1]);
			sender.append_to_msg_out(format_reply(RPL_INVITING, params[0], params[1]));
		}
	}
}
