/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:50 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/23 12:14:21 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

// TODO

StatusCode	Server::join(Client &sender, std::vector<std::string> const &parameters)
{
	(void)parameters;
	std::cout << "JOIN cmd called\n";
	if (sender.has_mode(AlreadySentPass) == false)
	{
		std::string message = ":ircserv Password required.\nTry /quote PASS <password>\n\r";
		send_message(sender.get_socket(), message);
	}
	return Success;
}
