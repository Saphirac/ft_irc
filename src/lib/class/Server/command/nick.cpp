/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/05 22:06:43 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <iostream>

/**
 * @brief Sends an ERR_NONICKNAMEGIVEN reply to a given client.
 *
 * @param sender The client to send the reply to.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_no_nickname_given(Client const &sender)
{
	std::string const message = format_reply(ERR_NONICKNAMEGIVEN);

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (sender.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	return false;
}

/**
 * @brief Sends an ERR_ERRONEUSNICKNAME reply to a given client.
 *
 * @param sender The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_erroneus_nickname(Client const &sender, std::string const &nickname)
{
	std::string const message = format_reply(ERR_ERRONEUSNICKNAME, nickname.c_str());

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (sender.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	return false;
}

/**
 * @brief Sends an ERR_NICKNAMEINUSE reply to a given client.
 *
 * @param sender The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return true if a fatal error occured, false otherwise.
 */
inline static bool error_nickname_in_use(Client const &sender, std::string const &nickname)
{
	std::string const message = format_reply(ERR_NICKNAMEINUSE, nickname.c_str());

	if (message.empty())
	{
		std::cerr << "format_reply() failed\n";
		return true;
	}
	if (sender.send_message(message) == -1)
	{
		std::cerr << "Client::send_message() failed\n";
		return true;
	}
	return false;
}

/**
 * @brief Changes the nickname of a given client.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return true if a fatal error occured, false otherwise.
 */
bool Server::nick(Client &sender, std::string const &parameters)
{
	std::string const nickname =
		parameters[0] == ':' ? parameters.substr(1) : parameters.substr(0, parameters.find(' '));

	if (nickname.empty())
		return error_no_nickname_given(sender);

	if (nickname.size() > 9 || (LETTER + SPECIAL).find(nickname[0]) == std::string::npos
	    || nickname.find_first_not_of(LETTER + DIGIT + SPECIAL, 1) != std::string::npos)
		return error_erroneus_nickname(sender, nickname);

	if (this->_clients_by_nickname.count(nickname))
		return error_nickname_in_use(sender, nickname);

	this->_clients_by_nickname.erase(sender.get_nickname());
	sender.set_nickname(nickname);
	this->_clients_by_nickname[nickname] = &sender;

	return false;
}