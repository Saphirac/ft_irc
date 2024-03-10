/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:39 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/10 02:43:56 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

void Server::_quit(Client &sender, std::vector<std::string> const &params)
{
	if (!params.empty())
		sender.append_to_msg_out(sender.prefix() + "ERROR :" + params[0]);
	else
		sender.append_to_msg_out(sender.prefix() + "ERROR :" + DEFAULT_QUIT_MSG);
	sender.set_mode(IsAboutToBeDisconnected);
}
