/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_such_channel.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 01:56:41 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:00:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_NOSUCHCHANNEL reply to a client.
 *
 * @param client The client to send the reply to.
 * @param channel_name The name of the channel that does not exist.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_no_such_channel(Client &client, ChannelName const &channel_name)
{
	std::string const msg = format_reply(ERR_NOSUCHCHANNEL, &channel_name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
