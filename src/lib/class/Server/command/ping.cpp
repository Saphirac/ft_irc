/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 01:38:29 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Sends a response to the PING command by sending a PONG with the corresponding token
 *
 * @param sender the client sending the ping
 * @param params the parameters containing the ping token
 * @return StatusCode depending on the success of the command
 */
void Server::_ping(Client &sender, std::vector<std::string> const &params)
{
	if (params.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PING");

	sender.append_to_msg_out(sender.prefix() + this->_name + "PONG " + params[0]);
}
