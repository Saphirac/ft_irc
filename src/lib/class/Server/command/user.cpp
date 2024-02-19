/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/07 10:56:04 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <cstdlib>

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
 * @brief Closes the connection of a given client and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to remove.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_no_nickname_set(Server &server, Client const &client)
{
	server.remove_client(client);
	return Success;
}

/**
 * @brief Closes the connection of a given client and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to remove.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_not_authenticated(Server &server, Client const &client)
{
	server.remove_client(client);
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
	std::string const message = format_reply(ERR_NEEDMOREPARAMS, "USER");

	if (message.empty())
		return ErrorFormatReply;

	if (client.send_message(message) == -1)
		return ErrorClientSendMessage;

	server.remove_client(client);
	return Success;
}

/**
 * @brief Checks whether a given username is invalid.
 *
 * @param username The username to check.
 *
 * @return true if the given username is invalid, false otherwise.
 */
inline static bool is_invalid_username(std::string const &username) { return username.find('@') != std::string::npos; }

/**
 * @brief Checks whether a given user mode mask is invalid.
 *
 * @param umode_mask The user mode mask to check.
 * @param umodes The `uint8_t` representation of the user mode mask.
 *
 * @return true if the given user mode mask is invalid, false otherwise.
 */
inline static bool is_invalid_umode_mask(std::string const &umode_mask, uint8_t const umodes)
{
	return umode_mask.size() > 3 || umodes & ~12;
}

/**
 * @brief Checks whether a given hostname is invalid.
 *
 * @param hostname The hostname to check.
 *
 * @return true if the given hostname is invalid, false otherwise.
 */
inline static bool is_invalid_hostname(std::string const &hostname)
{
	size_t pos = 0;

	do
	{
		std::string const shortname = hostname.substr(pos, hostname.find('.'));

		if ((letter + digit).find(shortname[0]) == std::string::npos
		    || shortname.find_first_not_of(letter + digit + "-", 1) != std::string::npos)
			return true;

		pos += shortname.size() + 1;
	}
	while (hostname.find('.') != std::string::npos);

	return false;
}

/**
 * @brief Closes the connection of a given client and removes them from the list of known clients.
 *
 * @param server The server to remove the client from.
 * @param client The client to remove.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_erroneus_argument(Server &server, Client const &client)
{
	server.remove_client(client);
	return Success;
}

/**
 * @brief Sends a RPL_WELCOME, a RPL_YOURHOST, a RPL_CREATED, and a RPL_MYINFO to a given client.
 *
 * @param client The client to send the replies to.
 * @param server_name The name of the server that the client is connected to.
 * @param server_version The version of the server that the client is connected to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode reply_welcome(
	Client            &client,
	std::string const &server_name,
	std::string const &server_version,
	std::string const &server_creation_date)
{
	std::string const messages[3] = {
		format_reply(RPL_WELCOME, client.user_mask()),
		format_reply(RPL_YOURHOST, server_name, server_version),
		format_reply(RPL_CREATED, server_creation_date),
	};
	size_t const messages_len = sizeof(messages) / sizeof(*messages);

	for (size_t i = 0; i < messages_len; ++i)
		if (messages[i].empty())
			return ErrorFormatReply;

	for (size_t i = 0; i < messages_len; ++i) client.append_message(messages[i]);
	return Success;
}

/**
 * @brief
 * Finilize the user registration process for a given client,
 * saving their username, their initial modes, their hostname, and their realname.
 *
 * @param client The client to finalize the registration for.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
StatusCode Server::user(Client &client, std::string const &parameters)
{
	if (client.has_mode(AlreadySentUser))
		return error_already_registered(client);

	if (!this->_password.empty() && !client.has_mode(AlreadySentPass))
		return error_not_authenticated(*this, client);

	if (client.get_nickname().empty())
		return error_no_nickname_set(*this, client);

	size_t            pos = 0;
	std::string const first_parameters = parameters.substr(0, parameters.find(" :"));
	std::string const username = first_parameters.substr(pos, first_parameters.find(' ', pos));

	pos += username.size() + 1;

	std::string const umode_mask = first_parameters.substr(pos, first_parameters.find(' ', pos));

	pos += umode_mask.size() + 1;

	std::string const hostname = first_parameters.substr(pos, first_parameters.find(' ', pos));

	pos += hostname.size() + 1;

	std::string const realname = parameters.size() == first_parameters.size()
	                               ? first_parameters.substr(pos, first_parameters.find(' ', pos))
	                               : parameters.substr(first_parameters.size() + 2);

	if (username.empty() || umode_mask.empty() || hostname.empty() || realname.empty())
		return error_need_more_parameters(*this, client);

	uint8_t const umodes = static_cast<uint8_t>(strtol(umode_mask.c_str(), NULL, 10));

	if (is_invalid_username(username) || is_invalid_umode_mask(umode_mask, umodes) || is_invalid_hostname(hostname))
		return error_erroneus_argument(*this, client);

	client.set_username(username);
	client.set_hostname(hostname);
	client.set_realname(realname);
	if (umodes & 1 << 2)
		client.set_mode(Invisible);
	if (umodes & 1 << 3)
		client.set_mode(WallopsListener);
	client.set_mode(AlreadySentUser);
	return reply_welcome(client, this->_name, this->_version, this->_creation_date);
}
// TODO: implement unit tests for this function