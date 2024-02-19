/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:22:46 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/17 15:02:07 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

/**
 * @brief send a message to a client
 *
 * // TODO : change exit failure with exception
 * @param client_socket
 * @param message
 */
void	send_message(int client_socket, std::string message)
{
	if (send(client_socket, message.c_str(), message.size(), 0) == -1)
	{
		std::cerr << "Problem with send()." << std::endl;
		exit(EXIT_FAILURE);
	}
}
