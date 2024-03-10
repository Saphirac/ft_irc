/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:06:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/10 06:54:29 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Message.hpp"
#include "class/Server.hpp"
#include "class/exception/ProblemWithAccept.hpp"
#include "class/exception/ProblemWithEpollCtl.hpp"
#include "class/exception/ProblemWithEpollWait.hpp"
#include "class/exception/ProblemWithFcntl.hpp"
#include "class/exception/ProblemWithRecv.hpp"
#include "replies.hpp"
#include <fcntl.h>
#include <sys/epoll.h>

bool interrupted = false;

/**
 * @brief Starts the server, making it receiving messages, handling them and sending the appropriate replies.
 */
void Server::start(void)
{
	while (!interrupted)
	{
		this->_handle_epoll_events();
		for (std::map<int, Client>::iterator it = this->_clients_by_socket.begin();
		     it != this->_clients_by_socket.end();)
		{
			Client &client = it->second;

			this->_compute_next_msg_for_a_client(client);
			client.send_msg_out();
			// TODO: check if the client connection is still active.
			++it;
			if (client.has_mode(IsAboutToBeDisconnected))
				this->_remove_client(client);
		}
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

#define EPOLL_WAIT_TIMEOUT 100
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

	fds_ready = epoll_wait(this->_epoll_socket, events, MAX_CLIENTS, EPOLL_WAIT_TIMEOUT);
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
	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
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
 * @brief Extracts the next message from the input buffer of a client, and calls the appropriate command.
 *
 * @param client The client for which the next message will be extracted from the input buffer.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_compute_next_msg_for_a_client(Client &client)
{
	std::string const raw_msg = client.get_next_msg();

	if (!raw_msg.empty())
	{
		Message const         msg(raw_msg);
		
		std::string const    &command_name = msg.get_command();
		CommandIterator const command_by_name = this->_commands_by_name.find(command_name);

		if (command_by_name == this->_commands_by_name.end())
			client.append_to_msg_out(client.formatted_reply(ERR_UNKNOWNCOMMAND, &command_name));
		else
			(this->*(command_by_name->second))(client, msg.get_parameters());
	}
}
