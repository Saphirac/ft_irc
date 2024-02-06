/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/06 01:26:54 by jodufour         ###   ########.fr       */
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
 * @return 0 upon success, or a positive error code upon failure.
 */
inline static StatusCode error_no_nickname_given(Client const &sender)
{
	std::string const message = format_reply(ERR_NONICKNAMEGIVEN);

	if (message.empty())
		return ErrorFormatReply;

	if (sender.send_message(message) == -1)
		return ErrorClientSendMessage;

	return Success;
}

/**
 * @brief Sends an ERR_ERRONEUSNICKNAME reply to a given client.
 *
 * @param sender The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return 0 upon success, or a positive error code upon failure.
 */
inline static StatusCode error_erroneus_nickname(Client const &sender, std::string const &nickname)
{
	std::string const message = format_reply(ERR_ERRONEUSNICKNAME, nickname.c_str());

	if (message.empty())
		return ErrorFormatReply;

	if (sender.send_message(message) == -1)
		return ErrorClientSendMessage;

	return Success;
}

/**
 * @brief Sends an ERR_NICKNAMEINUSE reply to a given client.
 *
 * @param sender The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return 0 upon success, or a positive error code upon failure.
 */
inline static StatusCode error_nickname_in_use(Client const &sender, std::string const &nickname)
{
	std::string const message = format_reply(ERR_NICKNAMEINUSE, nickname.c_str());

	if (message.empty())
		return ErrorFormatReply;

	if (sender.send_message(message) == -1)
		return ErrorClientSendMessage;

	return Success;
}

/**
 * @brief Changes the nickname of a given client.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return 0 upon success, or a positive error code upon failure.
 */
StatusCode Server::nick(Client &sender, std::string const &parameters)
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

	if (sender.send_message("NICK " + nickname) == -1)
		return ErrorClientSendMessage;

	return Success;
}