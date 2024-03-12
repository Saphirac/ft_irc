/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 05:29:01 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Gets information about either specific channels, or all the channels on the server.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_list(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);
	(void)parameters;
	// TODO
}
