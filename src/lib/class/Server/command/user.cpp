/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/14 00:13:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef DEBUG
#	include <iostream>
#endif

// REMIND: this part is commented out because `irssi` doesn't send the user mode mask, it sucks!
// #include "class/specialized_string/UserModeMask.hpp"
// #include <cstdlib>
#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief
 * Finalizes the user registration process for a client,
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
void Server::_user(Client &sender, CommandParameterVector const &parameters)
{
	if (sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_ALREADYREGISTERED);
	if (!this->_password.empty() && !sender.has_mode(Authenticated))
		return sender.append_formatted_reply_to_msg_out(ERR_PASSWDMISMATCH);
	if (parameters.size() < 4)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "USER");

	UserName const &username = parameters[0];

	if (!username.is_valid())
		return sender.set_mode(IsAboutToBeDisconnected);

	// REMIND: this part is commented out because `irssi` doesn't send the user mode mask, it sucks!
	// UserModeMask const &umode_mask = parameters[1];
	// uint8_t const       umodes = strtol(umode_mask.c_str(), NULL, 10);

	// if (!umode_mask.is_valid(umodes))
	// 	return sender.set_mode(IsAboutToBeDisconnected);

	HostName const &hostname = parameters[2];

	if (!hostname.is_valid())
		return sender.set_mode(IsAboutToBeDisconnected);

	RealName const &realname = parameters[3];

	sender.set_username(username);
	sender.set_hostname(hostname);
	sender.set_realname(realname);
	// REMIND: this part is commented out because `irssi` doesn't send the user mode mask, it sucks!
	// if (umodes & 1 << 2)
	// 	sender.set_mode(Invisible);
	// if (umodes & 1 << 3)
	// 	sender.set_mode(WallopsListener);
	sender.set_mode(AlreadySentUser);

	if (sender.get_nickname().is_valid())
		this->_welcome(sender);
}
// TODO: implement unit tests for this function
