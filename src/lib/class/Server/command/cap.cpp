/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:43:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/11 00:26:33 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Handle the CAP command used by the client to negotiate capabilities
 * Here we send CAP * LS :none to the client because we don't support any capability
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_cap(
	Client                         &sender __attribute__((unused)),
	std::vector<std::string> const &parameters __attribute__((unused)))
{
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "CAP");

	if (parameters[0] == "LS")
		sender.append_to_msg_out("CAP * LS :none");
}
