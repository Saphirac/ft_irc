/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/02 04:19:57 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Handle the receiving of a PONG msg : if the client has been pinged before, and the pong token matches the
 * previous ping token, it will set back the has_been_pinged flag to false and the time_last_msg to the current time.
 * Else it will simply return without doing anything.
 *
 * The PONG command takes as params the server name and the token of the ping.
 *
 * @param sender the client sending the pong
 * @param params the parameters of the pong
 *
 * // TODO : put the method back as void when all command are back to void
 */
StatusCode Server::pong(Client &sender, std::vector<std::string> const &params)
{
	if (sender.get_has_been_pinged() == false)
		return;
	if (params[0].empty() || (params[0] && params[0] != this->_name))
	{
		std::string msg = format_reply(ERR_NEEDMOREPARAMS, "PONG");
		sender.append_to_msg_out(msg);
		return;
	}
	if (!sender.get_ping_token().empty() && (param[1].empty() || params[1] != sender.get_ping_token()))
		return;
	sender.set_has_been_pinged(false);
	sender.set_time_last_msg();
}
