/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 12:54:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Makes a user invite another user to a channel.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_invite(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	if (parameters.size() < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "INVITE");

	NickName const           &target_nickname = parameters[0];
	ClientMap::iterator const user_by_nickname = this->_clients_by_nickname.find(target_nickname);

	if (user_by_nickname == this->_clients_by_nickname.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHNICK, &target_nickname);

	Client                    &target = *user_by_nickname->second;
	ChannelName const         &channel_name = parameters[1];
	ChannelMap::iterator const channel_by_name = this->_channels_by_name.find(channel_name);

	if (channel_by_name == this->_channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_name);

	Channel &channel = channel_by_name->second;

	if (!channel.has_member(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTONCHANNEL, &channel_name);
	if (channel.has_member(target))
		return sender.append_formatted_reply_to_msg_out(ERR_USERONCHANNEL, &target_nickname, &channel_name);

	Channel::Modes const &channel_modes = channel.get_modes();
	bool const            is_sender_operator = channel_modes.has_operator(sender);

	if (channel_modes.is_set(InviteOnly) && !is_sender_operator)
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &channel_name);

	Client::Modes const &target_modes = target.get_modes();

	if (target_modes.is_set(Away))
		return sender.append_formatted_reply_to_msg_out(RPL_AWAY, &target_nickname, &target_modes.get_away_msg());

	channel.add_invited_user(target, is_sender_operator);
	target.append_to_msg_out(sender.prefix() + "INVITE " + target_nickname + " :" + channel_name);
	sender.append_formatted_reply_to_msg_out(RPL_INVITING, &target_nickname, &channel_name);
}
