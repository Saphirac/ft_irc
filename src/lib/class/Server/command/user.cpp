/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/01 00:04:09 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelMode.hpp"
#include "class/Server.hpp"
#include "class/UserModeMask.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <cstdlib>

/**
 * @brief
 * Finilizes the user registration process for a client,
 * saving their username, their initial modes, their hostname, and their realname.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 * @throw `ProblemWithSend` if the `send()` function fails.
 */
void Server::user(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(format_reply(ERR_ALREADYREGISTERED));
	if ((!this->_password.empty() && !sender.has_mode(AlreadySentPass)) || sender.get_nickname().empty())
		return this->remove_client(sender);
	if (parameters.size() < 4)
	{
		sender.append_to_msg_out(format_reply(ERR_NEEDMOREPARAMS, "USER"));
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

	std::string const user_mask = sender.user_mask();

	sender.append_to_msg_out(format_reply(RPL_WELCOME, &user_mask));
	sender.append_to_msg_out(format_reply(RPL_YOURHOST, &this->_name, &this->_version));
	sender.append_to_msg_out(format_reply(RPL_CREATED, &this->_creation_date));
	sender.append_to_msg_out(format_reply(RPL_MYINFO, &this->_name, &this->_version, USER_MODES, CHANNEL_MODES));
}
// TODO: implement unit tests for this function
