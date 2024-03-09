/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:45 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/06 02:21:43 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Promotes a client to local operator.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_oper(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.size() < 2)
		return sender.append_to_msg_out(sender.formatted_reply(ERR_NEEDMOREPARAMS, "OPER"));

	UserName const &username = parameters[0];

	if (!username.is_valid()) // Erroneus username: silently abort the command
		return;

	if (this->_operator_hosts.find(sender.get_hostname()) == this->_operator_hosts.end())
		return sender.append_to_msg_out(sender.formatted_reply(ERR_NOOPERHOST));

	std::string const                                             &password = parameters[1];
	std::map<std::string, std::string const>::const_iterator const id = this->_operator_ids.find(username);

	if (id == this->_operator_ids.end() || password != id->second)
		return sender.append_to_msg_out(sender.formatted_reply(ERR_PASSWDMISMATCH));

	sender.set_mode(LocalOperator);

	sender.append_to_msg_out(sender.formatted_reply(RPL_YOUREOPER));
	sender.append_to_msg_out(sender.prefix() + "MODE " + sender.get_nickname() + " +O");
}
// TODO: implement unit tests for this function
