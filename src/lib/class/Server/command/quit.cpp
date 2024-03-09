/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:39 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 13:03:25 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO

void Server::quit(Client &sender, std::vector<std::string> const &params)
{
	if (params[0])
		sender->disconnect(params[0]);
	else
		sender->disconnect("QUIT command received, you are being disconnected.");
}
