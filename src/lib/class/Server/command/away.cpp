/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:09 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 19:28:33 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends a RPL_UNAWAY to a client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode reply_no_longer_away(Client &client)
{
	std::string const msg = format_reply(RPL_UNAWAY);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Sends a RPL_NOWAWAY to a client.
 *
 * @param client The client to send the reply to.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
inline static StatusCode reply_now_away(Client &client)
{
	std::string const msg = format_reply(RPL_NOWAWAY);

	if (msg.empty())
		return ErrorFormatReply;

	client.append_to_msg_out(msg);
	return Success;
}

/**
 * @brief Marks/Unmarks a user as being away + Sets/Unsets their away message.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters that were passed to the command.
 *
 * @return A positive error code in case of an internal error. Otherwise, returns zero.
 */
StatusCode Server::away(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
	{
		sender.set_away_msg(Client::_default_away_msg);
		sender.clear_mode(Away);

		return reply_no_longer_away(sender);
	}

	std::string const &away_msg = parameters[0];

	sender.set_away_msg(away_msg);
	sender.set_mode(Away);

	return reply_now_away(sender);
}
// TODO: Implement unit tests for this function
