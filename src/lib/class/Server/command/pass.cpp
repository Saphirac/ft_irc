/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:14:19 by jodufour         ###   ########.fr       */
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
 * @throws
 * `ProblemWithFormatReply` if the reply message cannot be formatted.
 * `PriblemWithSend` if the reply message cannot be sent.
 */
void Server::pass(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentPass))
		return error_already_registered(sender);
	if (parameters.empty())
	{
		error_need_more_arguments(sender, "PASS");
		sender.send_msg_out();
		this->remove_client(sender);
		return;
	}

	std::string const &password = parameters[0];

	if (password != this->_password)
	{
		error_password_mismatch(sender);
		sender.send_msg_out();
		this->remove_client(sender);
		return;
	}

	sender.set_mode(AlreadySentPass);
}
// TODO: implement unit tests for this function
