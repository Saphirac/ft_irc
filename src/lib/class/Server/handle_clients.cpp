/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:11:16 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 18:54:38 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "class/Exceptions.hpp"
#include "class/Server.hpp"
#include "replies.hpp"
#include <fcntl.h>

/**
 * @brief Change the socket flag to non blocking
 *
 * @param socket the socket to set non blocking
 *
 * @throw ProblemWithFcntlFlags() if fcntl() fails in getting the flags
 * @throw ProblemWithSettingNonBlock() if fcntl() fails in setting the socket to non blocking
 */
void control_socket(int const socket)
{
	int flags = fcntl(socket, F_GETFL, 0);

	if (flags == -1)
		throw ProblemWithFcntlFlags();
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) == -1)
		throw ProblemWithSettingNonBlock();
}

/**
 * @brief call recv() to get what the client wrote in its socket
 *
 * @param client the client to get the message from
 *
 * @throw ProblemWithRecv() if recv() fails
 * @throw std::string() can throw exceptions depending on the error
 * @throw disconnect() can throw ProblemWithClose() if the close() function fails.
 */
void Server::rcv_client_event(Client *client)
{
	client->set_time_last_msg();

	char    buffer[4096];
	ssize_t bytes_read;

	bytes_read = recv(client->get_socket(), buffer, 4096, 0);
	if (bytes_read == -1)
		throw ProblemWithRecv();
	if (bytes_read == 0)
	{
		client->disconnect();
		this->remove_client(client);
		return;
	}
	client->append_to_msg_in(std::string(buffer, bytes_read));
}

/**
 * @brief parse the message and call the corresponding received command
 *
 * @param client to handle the event of
 * @param msg the msg to treat
 */
void Server::handle_client_event(Client *client, std::string msg)
{
	IrcMessage rcv_msg;

	rcv_msg.parse_irc_message(msg);

	std::string const                                            cmd = rcv_msg.get_command();
	std::map<std::string, Server::Command>::const_iterator const cmd_func = this->_map_of_cmds.find(cmd);

	if (cmd_func == this->_map_of_cmds.end())
	{
		std::string const msg_to_send = format_reply(ERR_UNKNOWNCOMMAND, &cmd);
		if (!msg_to_send.empty())
			client->append_to_msg_out(msg_to_send);
		return;
	}
	(this->*(cmd_func->second))(*client, rcv_msg.get_params());
}

/**
 * @brief add new client to the list of clients and add it's socket to the epoll list
 *
 * @throw ProblemWithAccept() if accept() fails
 * @throw control_socket() and ctrl_epoll_add() can throw different exceptions depending on the error
 * @throw new can throw different exceptions depending on the error
 */
void Server::handle_new_connection()
{
	int const client_socket = accept(this->_socket, NULL, NULL);

	if (client_socket == -1)
		throw ProblemWithAccept();
	control_socket(client_socket);
	this->_clients_socket[client_socket] = new Client(client_socket);
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients_socket[client_socket]->get_mut_epoll_event());
}
