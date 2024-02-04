/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/04 05:17:43 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserMode.hpp"
#include "class/Client.hpp"
#include "libft_irc.hpp"
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

// TODO: implement unit tests for this function
/**
 * @brief Sends an ERR_NEEDMOREPARAMS reply to a given client, and closes their connection.
 *
 * @param client The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_need_more_parameters(Client &client)
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
	client.disconnect();
	return false;
}

/**
 * @brief Sends an ERR_PASSWDMISMATCH reply to a given client, and closes their connection.
 *
 * @param client The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_password_mismatch(Client &client)
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
	client.disconnect();
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
bool pass(Client &sender, std::string const &parameters)
{
	if (sender.has_already_sent_pass())
		return error_already_registered(sender);

	if (parameters.empty())
		return error_need_more_parameters(sender);

	std::string password = parameters[0] == ':' ? parameters.substr(1) : parameters.substr(0, parameters.find(' '));

	if (password != SERVER_PASSWORD)
		return error_password_mismatch(sender);

	sender.set_modes(1 << UserModePass);

	return false;
}
