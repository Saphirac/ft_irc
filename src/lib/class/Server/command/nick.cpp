/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 19:26:21 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <iostream>

/**
 * @brief Sends an ERR_NONICKNAMEGIVEN to a client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_no_nickname_given(Client &client)
{
	std::string const msg = format_reply(ERR_NONICKNAMEGIVEN);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Sends an ERR_ERRONEUSNICKNAME to a client.
 *
 * @param client The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_erroneus_nickname(Client &client, std::string const &nickname)
{
	std::string const msg = format_reply(ERR_ERRONEUSNICKNAME, &nickname);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Sends an ERR_NICKNAMEINUSE to a client.
 *
 * @param client The client to send the reply to.
 * @param nickname The nickname that caused the error.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_nickname_in_use(Client &client, std::string const &nickname)
{
	std::string const msg = format_reply(ERR_NICKNAMEINUSE, &nickname);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Changes the nickname of a user.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
StatusCode Server::nick(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
		return error_no_nickname_given(sender);

	Nickname const &nickname = parameters[0];

	if (!nickname.is_valid())
		return error_erroneus_nickname(sender, nickname);
	if (this->_clients_by_nickname.count(nickname))
		return error_nickname_in_use(sender, nickname);

	this->_clients_by_nickname.erase(sender.get_nickname());
	sender.set_nickname(nickname);
	this->_clients_by_nickname[nickname] = &sender;

	sender.append_to_msg_out("NICK " + nickname);
	return Success;
}
// TODO: implement unit tests for this function
