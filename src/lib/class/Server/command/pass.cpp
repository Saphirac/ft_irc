/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:01:08 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief
 * Checks if the given password matches the configured one for the connection to the server.
 * The result will determine whether the client is allowed to finilise their connection.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `ProblemWithSend` if the `send()` function fails.
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_pass(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentPass))
		return sender.append_to_msg_out(format_reply(ERR_ALREADYREGISTERED));
	if (parameters.empty())
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "PASS"));
		return this->_remove_client(sender);
	}

	std::string const &password = parameters[0];

	if (password != this->_password)
	{
		sender.append_to_msg_out(format_reply(ERR_PASSWDMISMATCH));
		return this->_remove_client(sender);
	}

	sender.set_mode(AlreadySentPass);
}
// TODO: implement unit tests for this function
