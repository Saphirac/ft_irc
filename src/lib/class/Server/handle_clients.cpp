/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:11:16 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/21 00:56:42 by mcourtoi         ###   ########.fr       */
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
	IrcMessage test = parse_irc_message(std::string(buffer));
	test.display();
	if (test.get_command() == "CAP")
		this->cap(*client, *test.get_params());
	else if (test.get_command() == "JOIN")
		this->join(*client, *test.get_params());
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
	this->_clients_socket[client_socket] = new Client(client_socket, "", "", "", "",  0);
	this->_clients_socket[client_socket]->set_epoll_event();
	printf("epoll_fd : %d\n client_socket : %d\n", this->_epoll_socket, client_socket);
	printf("epoll event adress : %p\n", this->_clients_socket[client_socket]->get_epoll_event());
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients_socket[client_socket]->get_epoll_event());
}
