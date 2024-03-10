/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:25:39 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/09 21:56:57 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

#define DEFAULT_QUIT_MESSAGE "Leaving, bye! <3"
/**
 * @brief
 * Makes a user leave every channel they are in,
 * then disconnects them from the server
 * and removes them from the list of known clients.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_quit(Client &sender, std::vector<std::string> const &parameters)
{
	std::string const &quit_message = parameters.empty() ? DEFAULT_QUIT_MESSAGE : parameters[0];

	// TODO: Make the user leave every channel they are in, then disconnect them from the server and remove them from
	// the list of known clients.
	(void)sender;
	(void)quit_message;
}
