/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:21:15 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Changes the nickname of a user.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_nick(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
		return sender.append_to_msg_out(format_reply(ERR_NONICKNAMEGIVEN));

	NickName const &nickname = parameters[0];

	if (!nickname.is_valid())
		return sender.append_to_msg_out(format_reply(ERR_ERRONEUSNICKNAME, &nickname));
	if (this->_clients_by_nickname.count(nickname) != 0)
		return sender.append_to_msg_out(format_reply(ERR_NICKNAMEINUSE, &nickname));

	this->_clients_by_nickname.erase(sender.get_nickname());
	sender.set_nickname(nickname);
	this->_clients_by_nickname.insert(std::make_pair(nickname, &sender));

	sender.append_to_msg_out("NICK " + nickname);
}
// TODO: implement unit tests for this function
