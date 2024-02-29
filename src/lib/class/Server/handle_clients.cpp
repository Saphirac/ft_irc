/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clients.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:11:16 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 18:50:48 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"
#include "class/Server.hpp"
#include "replies.hpp"

/**
 * @brief Change the socket flag to non blocking
 *
 * @param socket the socket to set non blocking
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
	buffer[bytes_read] = '\0';
	client->append_to_msg_in(std::string(buffer));
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

	std::string cmd = rcv_msg.get_command();

	if (this->_map_of_cmds.find(cmd) == this->_map_of_cmds.end())
	{
		std::string const msg_to_send = format_reply(ERR_UNKNOWNCOMMAND, &rcv_msg.get_command());
		if (!msg_to_send.empty())
			client->append_to_msg_out(msg_to_send);
		return;
	}
	(this->*(_map_of_cmds[cmd]))(*client, rcv_msg.get_params());
}

/**
 * @brief add new client to the list of clients and add it's socket to the epoll list
 *
 */
void Server::handle_new_connection()
{
	int const client_socket = accept(this->_socket, NULL, NULL);

	if (client_socket == -1)
		throw ProblemWithAccept();
	control_socket(client_socket);
	this->_clients_socket[client_socket] = new Client(client_socket);
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients_socket[client_socket]->get_epoll_event());
}
