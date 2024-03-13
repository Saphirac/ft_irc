/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/13 06:21:56 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief
 * Checks if the given password matches the configured one for the connection to the server.
 * The result will determine whether the client is allowed to finilise their connection.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `ProblemWithSend` if the `send()` function fails.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_pass(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_ALREADYREGISTERED);
	if (parameters.empty())
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "PASS");

	std::string const &password = parameters[0];

	if (password != this->_password)
	{
		sender.clear_mode(Authenticated);
		return sender.append_formatted_reply_to_msg_out(ERR_PASSWDMISMATCH);
	}

	sender.set_mode(Authenticated);
}
// TODO: implement unit tests for this function
