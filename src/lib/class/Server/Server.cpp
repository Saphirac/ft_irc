/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/20 21:11:29 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "Message.hpp"
#include <cstdio>

/**
 * @brief create a loop to manage new incoming connections or messages
 *
 * @param myserver
 */
void Server::epoll_loop()
{
	int                fds_ready;
	struct epoll_event events[MAX_CLIENTS];

	fds_ready = epoll_wait(this->_epoll_socket, events, MAX_CLIENTS, -1);
	if (fds_ready == -1)
	{
		std::cerr << "Problem with epoll_wait() : " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < fds_ready; i++)
	{
		if (events[i].data.fd == this->_socket)
			handle_new_connection();
		else
			handle_client_event(this->_clients_socket[events[i].data.fd]);
	}
}

// Init server //
/**
 * @brief init server by creating a socket, binding it to a port and listening to it
 *
 * // TODO : throw exceptions bytes_readinstead of exit
 *
 */
void Server::init_socket_server()
{
	this->create_and_set_socket();
	this->bind_assign_sockaddr();
	this->set_sock_len();
	if (listen(this->_socket, 10) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::create_server()
{
	this->init_socket_server();
	this->_epoll_socket = create_epoll();
	printf("epoll_socket : %d\n", this->_epoll_socket);
	this->set_epoll_event();
	ctrl_epoll_add(this->_epoll_socket, this->_socket, this->_epoll_event);
	while (this->_shutdown == false)
	{
		epoll_loop();
	}
	// read_and_respond(connection);
}
