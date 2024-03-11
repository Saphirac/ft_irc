/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 02:49:26 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
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
 */
void Server::_pong(Client &sender, std::vector<std::string> const &params)
{
	if (sender.get_has_been_pinged() == false || params.empty() || sender.get_ping_token() != params[0])
		return;

	sender.set_has_been_pinged(false);
	sender.set_last_msg_time(clock());
}
