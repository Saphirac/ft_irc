/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   password_mismatch.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:41:53 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:02:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_PASSWDMISMATCH to a given client.
 *
 * @param client The client to send the reply to.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_password_mismatch(Client &client)
{
	std::string const msg = format_reply(ERR_PASSWDMISMATCH);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
