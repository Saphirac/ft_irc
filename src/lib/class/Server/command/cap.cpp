/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:43:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/05 01:28:47 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

/**
 * @brief Does nothing because we don't support any capability.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 */
void Server::_cap(
	Client                         &sender __attribute__((unused)),
	std::vector<std::string> const &parameters __attribute__((unused)))
{
}
