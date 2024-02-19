/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 23:55:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "channel_modes.hpp"
#include "class/Server.hpp"
#include "class/UserModeMask.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include "user_modes.hpp"
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
inline static StatusCode error_need_more_parameters(Server &server, Client &client)
{
	std::string const message = format_reply(ERR_NEEDMOREPARAMS, "USER");

	if (message.empty())
		return ErrorFormatReply;

	client.append_message(message);
	if (client.send_messages() == -1)
		return ErrorClientSendMessages;

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
	std::string const user_mask = client.user_mask();
	std::string const welcome = format_reply(RPL_WELCOME, &user_mask);

	if (welcome.empty())
		return ErrorFormatReply;

	std::string const your_host = format_reply(RPL_YOURHOST, &server_name, &server_version);

	if (your_host.empty())
		return ErrorFormatReply;

	std::string const created = format_reply(RPL_CREATED, &server_creation_date);

	if (created.empty())
		return ErrorFormatReply;

	std::string const my_info = format_reply(RPL_MYINFO, &server_name, &server_version, user_modes, channel_modes);

	if (my_info.empty())
		return ErrorFormatReply;

	client.append_message(welcome);
	client.append_message(your_host);
	client.append_message(created);
	client.append_message(my_info);
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
	Username const    username = first_parameters.substr(pos, first_parameters.find(' ', pos));

	if (username.empty())
		return error_need_more_parameters(*this, client);
	if (!username.is_valid())
		return error_erroneus_argument(*this, client);

	pos += username.size() + 1;

	UserModeMask const umode_mask = first_parameters.substr(pos, first_parameters.find(' ', pos));

	if (umode_mask.empty())
		return error_need_more_parameters(*this, client);

	uint8_t const umodes = static_cast<uint8_t>(strtol(umode_mask.c_str(), NULL, 10));

	if (!umode_mask.is_valid(umodes))
		return error_erroneus_argument(*this, client);

	pos += umode_mask.size() + 1;

	Hostname const hostname = first_parameters.substr(pos, first_parameters.find(' ', pos));

	if (hostname.empty())
		return error_need_more_parameters(*this, client);
	if (!hostname.is_valid())
		return error_erroneus_argument(*this, client);

	pos += hostname.size() + 1;

	Realname const realname = parameters.size() == first_parameters.size()
	                            ? first_parameters.substr(pos, first_parameters.find(' ', pos))
	                            : parameters.substr(first_parameters.size() + 2);

	if (realname.empty())
		return error_need_more_parameters(*this, client);

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
