/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:06:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:42:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Message.hpp"
#include "class/Server.hpp"
#include "class/exception/ProblemWithAccept.hpp"
#include "class/exception/ProblemWithEpollCtl.hpp"
#include "class/exception/ProblemWithEpollWait.hpp"
#include "class/exception/ProblemWithFcntl.hpp"
#include "class/exception/ProblemWithRecv.hpp"
#include "ft_irc.hpp"
#include "replies.hpp"
#include <fcntl.h>

/**
 * @brief Starts the server, making it receiving messages, handling them and sending the appropriate replies.
 */
void Server::start(void)
{
	this->_shutdown = false;
	while (this->_shutdown == false)
	{
		this->_handle_epoll_events();
		this->_compute_next_msg_for_each_client();
		for (std::map<int, Client>::iterator it = this->_clients_by_socket.begin();
		     it != this->_clients_by_socket.end();
		     ++it)
			it->second.send_msg_out();
	}
}

/**
 * @brief Copies a given Client instance to the list of known clients.
 *
 * @param client The Client instance to copy.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_add_client(Client const &client)
{
	std::pair<std::map<int, Client>::iterator, bool> ret =
		this->_clients_by_socket.insert(std::make_pair(client.get_socket(), client));

	if (ret.second)
		this->_clients_by_nickname.insert(std::make_pair(client.get_nickname(), &ret.first->second));
}

/**
 * @brief Removes a client from the list of known clients.
 *
 * @param client The Client instance to remove.
 *
 * @throw `ProblemWithClose` if `close()` fails.
 */
void Server::_remove_client(Client const &client)
{
	if (this->_clients_by_nickname.erase(client.get_nickname()) != 0)
		this->_clients_by_socket.erase(client.get_socket());
}

/**
 * @brief
 * Iterates over the file descriptors for which `epoll()` marks a new event
 * to handle both new incoming connections and received data from clients.
 *
 * @throw `ProblemWithEpollWait()` if `epoll_wait()` fails.
 * @throw `ProblemWithAccept` if `accept()` fails.
 * @throw `ProblemWithFcntlFlags` if `fcntl()` fails in getting the flags.
 * @throw `ProblemWithSettingNonBlock` if `fcntl()` fails in setting the socket to non blocking.
 * @throw `ProblemWithEpollCtl` if `epoll_ctl()` fails.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_handle_epoll_events(void)
{
	int         fds_ready;
	epoll_event events[MAX_CLIENTS];

	// TODO: change the `-1` to a timeout value, to not block here when nothing happens.
	fds_ready = epoll_wait(this->_epoll_socket, events, MAX_CLIENTS, -1);
	if (fds_ready == -1)
		throw ProblemWithEpollWait();

	for (int i = 0; i < fds_ready; ++i)
	{
		if (events[i].data.fd == this->_socket)
			this->_new_client_connection();
		else
			this->_receive_data_from_client(this->_clients_by_socket[events[i].data.fd]);
	}
}

/**
 * @brief Adds a new client to the list of known clients, and makes `epoll()` keep track of its socket.
 *
 * @throw `ProblemWithAccept` if `accept()` fails.
 * @throw `ProblemWithFcntl` if `fcntl()` fails.
 * @throw `ProblemWithEpollCtl` if `epoll_ctl()` fails.
 * @throw `ProblemWithRecv` if `recv()` fails
 * @throw `ProblemWithClose` if `close()` fails.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_new_client_connection(void)
{
	int const client_socket = accept(this->_socket, NULL, NULL);

	if (client_socket == -1)
		throw ProblemWithAccept();

	int flags = fcntl(client_socket, F_GETFL, 0);

	// TODO: do not get the default flags, only set the ones that are needed for us.
	if (flags == -1)
		throw ProblemWithFcntl();
	flags |= O_NONBLOCK;
	if (fcntl(client_socket, F_SETFL, flags) == -1)
		throw ProblemWithFcntl();

	Client     &client = this->_clients_by_socket[client_socket];
	epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = client_socket;

	client.set_socket(client_socket);
	if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, client_socket, &event) == -1)
		throw ProblemWithEpollCtl();
}

#define BUFFER_SIZE 4096
/**
 * @brief Get the data that a client has sent via its socket.
 *
 * @param client the client to get the message from
 *
 * @throw `ProblemWithRecv()` if `recv()` fails
 * @throw `ProblemWithClose()` if `close()` fails.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_receive_data_from_client(Client &client)
{
	char          buffer[BUFFER_SIZE];
	ssize_t const bytes_received = recv(client.get_socket(), buffer, BUFFER_SIZE, 0);

	if (bytes_received == -1)
		throw ProblemWithRecv();

	client.set_last_msg_time(clock());
	if (bytes_received == 0)
		return this->_remove_client(client);
	client.append_to_msg_in(std::string(buffer, bytes_received));
}

/**
 * @brief For each client, gets the next message from their input buffer, and calls the appropriate command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_compute_next_msg_for_each_client(void)
{
	for (std::map<int, Client>::iterator it = this->_clients_by_socket.begin(); it != this->_clients_by_socket.end();
	     ++it)
	{
		Client           &client = it->second;
		std::string const raw_msg = client.get_next_msg();

		if (!raw_msg.empty())
		{
			Message const         msg(raw_msg);
			std::string const    &command_name = msg.get_command();
			CommandIterator const command_by_name = this->_commands_by_name.find(command_name);

			if (command_by_name == this->_commands_by_name.end())
				client.append_to_msg_out(format_reply(ERR_UNKNOWNCOMMAND, &command_name));
			else
				(this->*(command_by_name->second))(client, msg.get_parameters());
		}
	}
}
