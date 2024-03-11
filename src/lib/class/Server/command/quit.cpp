/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:39 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 09:45:49 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

void Server::_quit(Client &sender, std::vector<std::string> const &params)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_to_msg_out(sender.prefix() + "You are not registered.");
	if (!params.empty())
		sender.append_to_msg_out(sender.prefix() + "ERROR :" + params[0]);
	else
		sender.append_to_msg_out(sender.prefix() + "ERROR :" + DEFAULT_QUIT_MSG);
	sender.set_mode(IsAboutToBeDisconnected);
}
