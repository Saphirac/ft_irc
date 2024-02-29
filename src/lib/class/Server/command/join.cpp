/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 18:59:01 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <iostream>

// TODO : everything

StatusCode	Server::join(Client &sender, std::vector<std::string> const &parameters)
{
	(void)parameters;
	std::cout << "JOIN cmd called\n";
	if (sender.has_mode(AlreadySentPass) == false)
	{
		std::string message = ":ircserv Password required.\nTry /quote PASS <password>\n\r";
		sender.append_to_msg_out(message);
	}
	return Success;
}
