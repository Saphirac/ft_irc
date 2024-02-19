/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:24:45 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 14:51:36 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_NEEDMOREPARAMS to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_need_more_parameters(Client &client)
{
	std::string const msg = format_reply(ERR_NEEDMOREPARAMS, "OPER");

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Silently aborts the OPER command due to an invalid username.
 *
 * @return Zero.
 */
inline static StatusCode error_erroneus_username(void) { return Success; }

/**
 * @brief Sends an ERR_NOOPERHOST to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_no_operator_host(Client &client)
{
	std::string const msg = format_reply(ERR_NOOPERHOST);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Sends an ERR_PASSWDMISMATCH to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode error_password_mismatch(Client &client)
{
	std::string const msg = format_reply(ERR_PASSWDMISMATCH);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Sends a RPL_YOUREOPER and a MODE message to a given client.
 *
 * @param client The client to send the replies to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode reply_you_are_operator(Client &client)
{
	std::string const msg = format_reply(RPL_YOUREOPER);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	client.append_to_msg_out("MODE " + client.get_nickname() + " +" + client.get_modes().to_string());
	return Success;
}

/**
 * @brief Promotes a given client to local operator.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of internal error. Otherwise, returns zero.
 */
StatusCode Server::oper(Client &sender, std::string const &parameters)
{
	size_t            pos = 0;
	std::string const first_parameters = parameters.substr(0, parameters.find(" :"));
	Username const    username = first_parameters.substr(0, first_parameters.find(' ', pos));

	if (username.empty())
		return error_need_more_parameters(sender);
	if (!username.is_valid())
		return error_erroneus_username();

	pos += username.size() + 1;

	std::string const password = parameters.size() == first_parameters.size()
	                               ? first_parameters.substr(pos, first_parameters.find(' ', pos))
	                               : parameters.substr(first_parameters.size() + 2);

	if (password.empty())
		return error_need_more_parameters(sender);

	if (this->_operator_hosts.find(sender.get_hostname()) == this->_operator_hosts.end())
		return error_no_operator_host(sender);

	std::map<std::string, std::string>::const_iterator id = this->_operator_ids.find(username);

	if (id == this->_operator_ids.end() || id->second != password)
		return error_password_mismatch(sender);

	sender.set_mode(LocalOperator);

	return reply_you_are_operator(sender);
}
// TODO: implement unit tests for this function
