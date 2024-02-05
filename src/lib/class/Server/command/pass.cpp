/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/05 20:40:09 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserMode.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <iostream>

// REMIND: This is a dirty way to store the server password. To be changed.
#define SERVER_PASSWORD "0000"

/**
 * @brief Sends an ERR_ALREADYREGISTERED reply to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_already_registered(Client const &client)
{
	std::string const message = format_reply(ERR_ALREADYREGISTERED);

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (client.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	return false;
}

/**
 * @brief
 * Sends an ERR_NEEDMOREPARAMS reply to a given client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_need_more_parameters(Server &server, Client &client)
{
	std::string const message = format_reply(ERR_NEEDMOREPARAMS, "PASS");

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (client.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	server.remove_client(client);
	return false;
}

/**
 * @brief
 * Sends an ERR_PASSWDMISMATCH reply to a given client, closes their connection,
 * and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_password_mismatch(Server &server, Client &client)
{
	std::string const message = format_reply(ERR_PASSWDMISMATCH);

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (client.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	server.remove_client(client);
	return false;
}

/**
 * @brief
 * Checks if the given password matches the configured one.
 * The result will determine whether the client is allowed to finilise their connection.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return true if a fatal error occured, false otherwise.
 */
bool Server::pass(Client &sender, std::string const &parameters)
{
	if (sender.has_already_sent_pass())
		return error_already_registered(sender);

	std::string const password =
		parameters[0] == ':' ? parameters.substr(1) : parameters.substr(0, parameters.find(' '));

	if (password.empty())
		return error_need_more_parameters(*this, sender);

	if (password != SERVER_PASSWORD)
		return error_password_mismatch(*this, sender);

	sender.set_modes(1 << UserModePass);

	return false;
}

// TODO: implement unit tests for this function