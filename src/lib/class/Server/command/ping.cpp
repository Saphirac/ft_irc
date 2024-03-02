/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/02 04:20:04 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends a response to the PING command by sending a PONG with the corresponding token
 *
 * @param sender the client sending the ping
 * @param params the parameters containing the ping token
 * @return StatusCode depending on the success of the command
 */
StatusCode Server::Ping(Client &sender, std::vector<std::string> const &params)
{
	if (params.empty())
	{
		std::string msg = format_reply(ERR_NEEDMOREPARAMS, "PING");
		sender.append_to_msg_out(msg);
		return Success;
	}
	sender.append_to_msg_out("PONG " + params[0]);
	return Success;
}
