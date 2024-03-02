/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:43:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/02 03:26:31 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO finish the command

#include "class/Server.hpp"
#include <iostream>

/**
 * @brief Handle the CAP command used by the client to negotiate capabilities
 * Here we send CAP * LS :none to the client because we don't support any capability
 * 
 * @param sender the client asking
 * @param parameters the parameters of the command
 * @return StatusCode : how the command went
 * 
 * // TODO : use the prefix method instead of harcoding the prefix
 */
StatusCode Server::cap(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
	{
		std::cerr << "Cap incomplete\n";
		return Success;
	}

	if (parameters.size() > 1 && parameters[0] == "LS" && parameters[1] == "302")
	{
		std::string message = ":ircserv CAP * LS :none\n\r";
		sender.append_to_msg_out(message);
	}
	return Success;
}
