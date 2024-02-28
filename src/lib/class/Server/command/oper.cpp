/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:45 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:10:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_NOOPERHOST to a client.
 *
 * @param client The client to send the reply to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void error_no_operator_host(Client &client)
{
	std::string const msg = format_reply(ERR_NOOPERHOST);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}

/**
 * @brief Sends a RPL_YOUREOPER and a MODE message to a client.
 *
 * @param client The client to send the replies to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
inline static void reply_you_are_operator(Client &client)
{
	std::string const msg = format_reply(RPL_YOUREOPER);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
	client.append_to_msg_out("MODE " + client.get_nickname() + " +O");
}

/**
 * @brief Promotes a client to local operator.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::oper(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.size() < 2)
		return error_need_more_arguments(sender, "OPER");

	UserName const &username = parameters[0];

	if (!username.is_valid()) // Erroneus username: silently abort the command
		return;

	if (this->_operator_hosts.find(sender.get_hostname()) == this->_operator_hosts.end())
		return error_no_operator_host(sender);

	std::string const                                             &password = parameters[1];
	std::map<std::string, std::string const>::const_iterator const id = this->_operator_ids.find(username);

	if (id == this->_operator_ids.end() || password != id->second)
		return error_password_mismatch(sender);

	sender.set_mode(LocalOperator);

	return reply_you_are_operator(sender);
}
// TODO: implement unit tests for this function
