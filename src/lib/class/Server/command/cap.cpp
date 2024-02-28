/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:43:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 17:32:46 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO finish the command

#include "class/Server.hpp"

StatusCode	Server::cap(Client &sender, std::vector<std::string> const &parameters)
{
	if (parameters.empty())
	{
		std::cerr << "Cap incomplete\n";
		return Success;
	}
	if (parameters[0] == "LS" && parameters[1] == "302")
	{
		std::string message = ":ircserv CAP * LS :none\n\r";
		send_message(sender.get_socket(), message);
	}
	return Success;
}
