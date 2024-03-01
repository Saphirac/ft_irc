/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:09 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 23:12:38 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"
#include "class/Server.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Marks/Unmarks a user as being away + Sets/Unsets their away message.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::away(Client &sender, std::vector<std::string> const &parameters)
{
	if (!parameters.empty())
	{
		std::string const &away_msg = parameters[0];

		sender.set_away_msg(away_msg);
		sender.set_mode(Away);

		return sender.append_to_msg_out(format_reply(RPL_NOWAWAY));
	}
	sender.set_away_msg(Client::_default_away_msg);
	sender.clear_mode(Away);

	return sender.append_to_msg_out(format_reply(RPL_UNAWAY));
}
// TODO: Implement unit tests for this function
