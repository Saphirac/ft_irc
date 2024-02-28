/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   need_more_arguments.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:10:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:03:41 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_NEEDMOREPARAMS to a client.
 *
 * @param client The client to send the reply to.
 * @param command The command that was not provided with enough arguments.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_need_more_arguments(Client &client, char const *const command)
{
	std::string const msg = format_reply(ERR_NEEDMOREPARAMS, command);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
