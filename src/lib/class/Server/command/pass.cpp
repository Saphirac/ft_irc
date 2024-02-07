/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/07 10:21:57 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <iostream>

/**
 * @brief Sends an ERR_ALREADYREGISTERED to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_already_registered(Client &client)
{
	std::string const message = format_reply(ERR_ALREADYREGISTERED);

	if (message.empty())
		return ErrorFormatReply;

	client.append_message(message);
	return Success;
}

/**
 * @brief
 * Sends an ERR_NEEDMOREPARAMS to a given client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_need_more_parameters(Server &server, Client const &client)
{
	std::string const message = format_reply(ERR_NEEDMOREPARAMS, "PASS");

	if (message.empty())
		return ErrorFormatReply;

	if (client.send_message(message) == -1)
		return ErrorClientSendMessage;

	server.remove_client(client);
	return Success;
}

/**
 * @brief
 * Sends an ERR_PASSWDMISMATCH to a given client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_password_mismatch(Server &server, Client const &client)
{
	std::string const message = format_reply(ERR_PASSWDMISMATCH);

	if (message.empty())
		return ErrorFormatReply;

	if (client.send_message(message) == -1)
		return ErrorClientSendMessage;

	server.remove_client(client);
	return Success;
}

/**
 * @brief
 * Checks if the given password matches the configured one.
 * The result will determine whether the client is allowed to finilise their connection.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
StatusCode Server::pass(Client &sender, std::string const &parameters)
{
	if (sender.has_mode(AlreadySentPass))
		return error_already_registered(sender);

	std::string const password =
		parameters[0] == ':' ? parameters.substr(1) : parameters.substr(0, parameters.find(' '));

	if (password.empty())
		return error_need_more_parameters(*this, sender);

	if (password != this->_password)
		return error_password_mismatch(*this, sender);

	sender.set_mode(AlreadySentPass);
	return Success;
}
// TODO: implement unit tests for this function