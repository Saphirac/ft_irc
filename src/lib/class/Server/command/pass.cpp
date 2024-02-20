/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 19:28:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <iostream>

/**
 * @brief Sends an ERR_ALREADYREGISTERED to a client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_already_registered(Client &client)
{
	std::string const msg = format_reply(ERR_ALREADYREGISTERED);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief
 * Sends an ERR_NEEDMOREPARAMS to a client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_need_more_parameters(Server &server, Client &client)
{
	std::string const msg = format_reply(ERR_NEEDMOREPARAMS, "PASS");

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);

	StatusCode const status = client.send_msg_out();

	if (status)
		return status;

	server.remove_client(client);
	return Success;
}

/**
 * @brief
 * Sends an ERR_PASSWDMISMATCH to a client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_password_mismatch(Server &server, Client &client)
{
	std::string const msg = format_reply(ERR_PASSWDMISMATCH);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);

	StatusCode const status = client.send_msg_out();

	if (status)
		return status;

	server.remove_client(client);
	return Success;
}

/**
 * @brief
 * Checks if the given password matches the configured one for the connection to the server.
 * The result will determine whether the client is allowed to finilise their connection.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
StatusCode Server::pass(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentPass))
		return error_already_registered(sender);
	if (parameters.empty())
		return error_need_more_parameters(*this, sender);

	std::string const &password = parameters[0];

	if (password != this->_password)
		return error_password_mismatch(*this, sender);

	sender.set_mode(AlreadySentPass);

	return Success;
}
// TODO: implement unit tests for this function
