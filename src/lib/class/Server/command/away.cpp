/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:09 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:29:46 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "class/exception/InvalidConversion.hpp"
#include "class/exception/UnknownReply.hpp"
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
void Server::_away(Client &sender, std::vector<std::string> const &parameters)
{
	if (!parameters.empty())
	{
		std::string const &away_msg = parameters[0];

		sender.set_mode(Away, &away_msg);

		return sender.append_to_msg_out(format_reply(RPL_NOWAWAY));
	}
	sender.clear_mode(Away);

	sender.append_to_msg_out(format_reply(RPL_UNAWAY));
}
// TODO: Implement unit tests for this function
