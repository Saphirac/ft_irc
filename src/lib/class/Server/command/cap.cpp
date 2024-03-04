/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:43:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 15:22:37 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO finish the command

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Handle the CAP command used by the client to negotiate capabilities
 * Here we send CAP * LS :none to the client because we don't support any capability
 *
 * @param sender the client asking
 * @param parameters the parameters of the command
 * @return StatusCode : how the command went
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
StatusCode Server::cap(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "CAP"));

	if (parameters.size() > 1 && parameters[0] == "LS" && parameters[1] == "302")
		sender.append_to_msg_out(':' + this->_name + "CAP * LS :none\n\r");
	return Success;
}
