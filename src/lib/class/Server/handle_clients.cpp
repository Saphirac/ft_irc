/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:11:16 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/26 17:38:46 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "IrcMessage.hpp"

void control_socket(int const socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << "Erreur lors de la récupération des flags du socket." << std::endl;
	}
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) == -1)
	{
		std::cerr << "Erreur lors du passage du socket en mode non bloquant." << std::endl;
	}
}

#include <stdio.h>

void Server::handle_client_event(Client *client)
{
	client->set_time_last_msg();
	char buffer[513];
	int  bytes_read;

	bytes_read = recv(client->get_socket(), buffer, 512, 0);
	if (bytes_read == -1)
	{
		std::cerr << "Problem with recv()." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bytes_read == 0)
	{
		std::cout << "Client disconnected." << std::endl;
		close(client->get_socket());
		return;
	}
	buffer[bytes_read] = '\0';
	printf("Received: [%s]\n", buffer);
	printf("Received: [%s]\n", std::string(buffer).c_str());
	IrcMessage rcv_msg;
	// if (rcv_msg.is_complete() == false)
	// {	
	// 	client->append_to_msg_in(std::string(buffer));
	// 	return; 
	// }
	printf("[%s]\n", rcv_msg.get_command().c_str());
	//TODO protect against unknown / empty command 
	//(this->*(_map_of_cmds[rcv_msg.get_command()]))(*client, rcv_msg.get_params());
}

/**
 * @brief add new client to the list of clients
 *
 * // TODO : change exit failure with exception
 * // TODO : use a client with a handshake bool to check if nickname and other received input is correct
 *
 */
void Server::handle_new_connection()
{
	int	const client_socket = accept(this->_socket, NULL, NULL);
	if (client_socket == -1)
	{
		std::cerr << "Problem with accept()." << std::endl;
		exit(EXIT_FAILURE);
	}
	control_socket(client_socket);
	this->_clients_socket[client_socket] = new Client(client_socket);
	this->_clients_socket[client_socket]->set_epoll_event();
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients_socket[client_socket]->get_epoll_event());
}
