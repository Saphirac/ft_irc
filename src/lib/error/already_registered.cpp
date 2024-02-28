/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   already_registered.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 08:47:17 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:01:13 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_ALREADYREGISTERED to a client.
 *
 * @param client The client to send the reply to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_already_registered(Client &client)
{
	std::string const msg = format_reply(ERR_ALREADYREGISTERED);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
