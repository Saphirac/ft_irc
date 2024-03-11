/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 08:31:16 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

/**
 * @brief Gets information about either specific channels, or all the channels on the server.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_list(Client &sender, std::vector<std::string> const &parameters)
{
	(void)sender;
	(void)parameters;
	// TODO
}
