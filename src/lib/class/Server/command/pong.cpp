/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 09:45:49 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Reset the time of the last activity of a client after that this client has responded to a PING by a PONG.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_pong(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(sender.prefix() + "You are not registered.");
	if (sender.get_has_been_pinged() == false || params.empty() || sender.get_ping_token() != params[0])
		return;

	sender.set_has_been_pinged(false);
	sender.set_last_msg_time(clock());
}
