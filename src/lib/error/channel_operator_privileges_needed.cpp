/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_operator_privileges_needed.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:35:19 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:01:52 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_CHANOPRIVSNEEDED reply to a client.
 *
 * @param client The client to send the reply to.
 * @param channel_name The name of the channel for which operator privileges are needed.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_channel_operator_privileges_needed(Client &client, ChannelName const &channel_name)
{
	std::string const msg = format_reply(ERR_CHANOPRIVSNEEDED, &channel_name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
