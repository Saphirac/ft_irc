/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 00:37:58 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <iostream>

// TODO : Change the password verification up ahead before executing any command	

void	Server::join(Client &sender, std::vector<std::string> const &parameters)
{
	if (sender.has_mode(AlreadySentPass) == false)
	{
		std::string message = ":ircserv Password required.\nTry /quote PASS <password>\n\r";
		sender.append_to_msg_out(message);
	}
	if (params.size() == 1 && params[0] == '0')
		sender->part_all_channels("Quitting all channels.");
}
