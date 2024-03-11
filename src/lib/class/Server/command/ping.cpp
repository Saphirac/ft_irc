/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 04:59:36 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Sends back a PONG to a client asking if the server connection is still alive.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_ping(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PING");

	sender.append_to_msg_out(sender.prefix() + " PONG " + this->_name + " :" + parameters[0]);
}
