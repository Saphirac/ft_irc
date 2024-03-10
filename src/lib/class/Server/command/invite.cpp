/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/10 06:16:34 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Invite a user to a channel, adding him to the Invite list
 *
 * @param sender the client inviting
 * @param params the command parameters (nickname, channel)
 */
void Server::_invite(Client &sender, std::vector<std::string> const &params)
{
	if (params.size() < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "INVITE");

	NickName const &nickname = sender.get_nickname();
	NickName const &user_to_invite_nickname = params[0];

	if (this->_clients_by_nickname.count(user_to_invite_nickname) == 0)
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &nickname);

	Client            &user_to_invite = *this->_clients_by_nickname[user_to_invite_nickname];
	ChannelName const &chan_name = params[1];

	if (this->_channels_by_name.count(chan_name) == 0)
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &params[1]);

	Channel       &chan = this->_channels_by_name[params[1]];
	Channel::Modes chan_modes = chan.get_modes();

	if (!chan.has_member(sender))
		sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &chan_name);
	else if (chan.has_member(user_to_invite))
		sender.append_formatted_reply_to_msg_out(ERR_USERONCHANNEL, &user_to_invite_nickname, &chan_name);
	else if (chan_modes.is_set(InviteOnly) && !chan_modes.has_operator(sender))
		sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &nickname);
	else
	{
		Client::Modes const &user_modes = user_to_invite.get_modes();
		if (user_modes.is_set(Away))
			sender.append_formatted_reply_to_msg_out(RPL_AWAY, &user_to_invite_nickname, &user_modes.get_away_msg());
		else
		{
			chan.set_mode(InviteMask, &user_to_invite_nickname);
			user_to_invite.append_to_msg_out(sender.prefix() + "INVITE : " + chan_name);
			sender.append_formatted_reply_to_msg_out(RPL_INVITING, &user_to_invite_nickname, &chan_name);
		}
	}
}
