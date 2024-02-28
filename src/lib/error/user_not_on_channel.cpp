/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_not_on_channel.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:53:12 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:03:18 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"

/**
 * @brief Sends an ERR_USERNOTONCHANNEL reply to a client.
 *
 * @param client The client to send the reply to.
 * @param channel_name The name of the channel the client is not on.
 * @param nickname The nickname of the client that is not on the channel.
 *
 * @throws `ProblemWithFormatReply` if the reply message cannot be formatted.
 */
void error_user_not_on_channel(Client &client, ChannelName const &channel_name, NickName const &nickname)
{
	std::string const msg = format_reply(ERR_USERNOTONCHANNEL, &nickname, &channel_name);

	if (msg.empty())
		throw ProblemWithFormatReply();

	client.append_to_msg_out(msg);
}
