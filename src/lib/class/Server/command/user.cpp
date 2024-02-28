/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:26:35 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelMode.hpp"
#include "class/Exceptions.hpp"
#include "class/Server.hpp"
#include "class/UserModeMask.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <cstdlib>

/**
 * @brief Sends a RPL_WELCOME, a RPL_YOURHOST, a RPL_CREATED, and a RPL_MYINFO to a client.
 *
 * @param client The client to send the replies to.
 * @param server_name The name of the server that the client is connected to.
 * @param server_version The version of the server that the client is connected to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void reply_welcome(
	Client            &client,
	std::string const &server_name,
	std::string const &server_version,
	std::string const &server_creation_date)
{
	std::string const user_mask = client.user_mask();
	std::string const welcome = format_reply(RPL_WELCOME, &user_mask);

	if (welcome.empty())
		throw ProblemWithFormatReply();

	std::string const your_host = format_reply(RPL_YOURHOST, &server_name, &server_version);

	if (your_host.empty())
		throw ProblemWithFormatReply();

	std::string const created = format_reply(RPL_CREATED, &server_creation_date);

	if (created.empty())
		throw ProblemWithFormatReply();

	std::string const my_info = format_reply(RPL_MYINFO, &server_name, &server_version, USER_MODES, CHANNEL_MODES);

	if (my_info.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(welcome);
	client.append_to_msg_out(your_host);
	client.append_to_msg_out(created);
	client.append_to_msg_out(my_info);
}

/**
 * @brief
 * Finilizes the user registration process for a client,
 * saving their username, their initial modes, their hostname, and their realname.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throws
 * `ProblemWithFormatReply` if the reply message cannot be formatted.
 * `PriblemWithSend` if the reply message cannot be sent.
 */
void Server::user(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentUser))
		return error_already_registered(sender);
	if ((!this->_password.empty() && !sender.has_mode(AlreadySentPass)) // Not authenticated
	    || sender.get_nickname().empty())                               // Missing a previous NICK command
		return this->remove_client(sender);
	if (parameters.size() < 4)
	{
		error_need_more_arguments(sender, "USER");
		sender.send_msg_out();
		this->remove_client(sender);
		return;
	}

	HostName const &hostname = parameters[0];

	if (!hostname.is_valid())
		return this->remove_client(sender);

	UserName const &username = parameters[1];

	if (!username.is_valid())
		return this->remove_client(sender);

	UserModeMask const &umode_mask = parameters[2];
	uint8_t const       umodes = strtol(umode_mask.c_str(), NULL, 10);

	if (!umode_mask.is_valid(umodes))
		return this->remove_client(sender);

	RealName const &realname = parameters[3];

	sender.set_username(username);
	sender.set_hostname(hostname);
	sender.set_realname(realname);
	if (umodes & 1 << 2)
		sender.set_mode(Invisible);
	if (umodes & 1 << 3)
		sender.set_mode(WallopsListener);
	sender.set_mode(AlreadySentUser);

	return reply_welcome(sender, this->_name, this->_version, this->_creation_date);
}
// TODO: implement unit tests for this function
