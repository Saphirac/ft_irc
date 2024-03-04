/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 18:51:36 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "Message.hpp"
#include "class/Exceptions.hpp"

/**
 * @brief check the msg_in field of the client, extract the message ( <= 512 char and finishing by a crlf) and handle it
 *
 */
void Server::handle_all_events_routine()
{
	for (std::map<int, Client *>::iterator it = this->_clients_socket.begin(); it != this->_clients_socket.end(); ++it)
	{
		std::string const next_msg = it->second->get_next_msg();

		if (next_msg.empty() == false)
			this->handle_client_event(it->second, next_msg);
	}
}

/**
 * @brief routine to send the msg waiting in the msg_out field of each client
 *
 * @throw ProblemWithSend() if send() fails
 */
void Server::send_msg_out()
{
	for (std::map<int, Client *>::iterator it = this->_clients_socket.begin(); it != this->_clients_socket.end(); ++it)
	{
		if (it->second->get_msg_out().empty() == false)
		{
			if (send(it->second->get_socket(), it->second->get_msg_out().c_str(), it->second->get_msg_out().size(), 0)
			    == -1)
				throw ProblemWithSend();
			it->second->clear_msg_out();
		}
	}
}

/**
 * @brief create a loop to manage new incoming connections or messages
 *
 * @throw ProblemWithEpollWait() if epoll_wait() fails
 * @throw handle_new_connection() and rcv_client_event() can throw different exceptions depending on the error
 */
void Server::epoll_loop()
{
	int                fds_ready;
	struct epoll_event events[MAX_CLIENTS];

	fds_ready = epoll_wait(this->_epoll_socket, events, MAX_CLIENTS, -1);
	if (fds_ready == -1)
		throw ProblemWithEpollWait();

	for (int i = 0; i < fds_ready; ++i)
	{
		if (events[i].data.fd == this->_socket)
			handle_new_connection();
		else
			rcv_client_event(this->_clients_socket[events[i].data.fd]);
	}
}

// Init server //
/**
 * @brief init server by creating a socket, binding it to a port and listening to it
 *
 * @throw ProblemWithListen() if listen() fails
 * @throw create_and_set_socket() and bind_assign_sockaddr() can throw different exceptions depending on the error
 */
void Server::init_socket_server()
{
	this->create_and_set_socket();
	this->bind_assign_sockaddr();
	this->set_sock_len();
	if (listen(this->_socket, 10) < 0)
		throw ProblemWithListen();
}

/**
 * @brief Main Server function : init the socket of the server, create the epoll socket and loop to receive and send
 * messages
 *
 * @throw ctrl_epoll_add(), init_socket_server(), create_epoll(), set_epoll_event(), ctrl_epoll_add(), epoll_loop(),
 * handle_all_events_routine() and send_msg_out() can throw different exceptions depending on the error
 * // TODO : handle signals to set shutdown
 */
void Server::create_server()
{
	this->init_socket_server();
	this->create_epoll();
	this->set_epoll_event();
	ctrl_epoll_add(this->_epoll_socket, this->_socket, this->_epoll_event);
	while (this->_shutdown == false)
	{
		epoll_loop();
		handle_all_events_routine();
		send_msg_out();
	}
}
