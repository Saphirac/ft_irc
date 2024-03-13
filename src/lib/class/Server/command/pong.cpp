/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/13 14:59:22 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Resets the time of the last activity of a client after that this client has responded to a PING by a PONG.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_pong(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.get_has_been_pinged() == false || parameters.empty() || sender.get_ping_token() != parameters[0])
		return;

	sender.set_has_been_pinged(false);
	sender.set_last_msg_time(time(NULL));
}
