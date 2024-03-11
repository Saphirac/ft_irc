/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:06:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 11:00:51 by jodufour         ###   ########.fr       */
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
#include <cstdlib>
#include <fcntl.h>
#include <sys/epoll.h>

// How many seconds since the last received message from a client the server will wait
// before sending a PING to that client.
#define TIMEOUT 180
// How many seconds since the last PING sent to a client the server will wait without receiving a PONG from that client
// before disconnecting that client.
#define TIMEOUT_SINCE_PING 15

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
void Server::_remove_client(Client &client, std::string const &quit_msg)
{
	// BUG: This implementation is incorrect, to be fixed
	if (!client.get_joined_channels_by_name().empty())
	{
		std::vector<std::string>                              channel_and_msg;
		std::map<ChannelName, Channel *const>::const_iterator my_end = client.get_joined_channels_by_name().end();

		for (std::map<ChannelName, Channel *const>::const_iterator cit = client.get_joined_channels_by_name().begin();
		     cit != my_end;
		     ++cit)
			channel_and_msg.push_back(cit->first);
		channel_and_msg.push_back(quit_msg);
		this->_part(client, channel_and_msg);
	}

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
	epoll_event events[MAXIMUM_NUMBER_OF_CLIENTS];

	fds_ready = epoll_wait(this->_epoll_socket, events, MAXIMUM_NUMBER_OF_CLIENTS, EPOLL_WAIT_TIMEOUT);
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

	if (client.get_has_been_pinged() == false)
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
		Message const msg(raw_msg);

		std::string const    &command_name = msg.get_command();
		CommandIterator const command_by_name = this->_commands_by_name.find(command_name);

		if (command_by_name == this->_commands_by_name.end())
			client.append_formatted_reply_to_msg_out(ERR_UNKNOWNCOMMAND, &command_name);
		else
			(this->*(command_by_name->second))(client, msg.get_parameters());
	}
}

inline static std::string const random_string(size_t len)
{
	std::string const available_characters =
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-*/!~@#$%^&()_+{}[]|;<>,.?/";
	size_t const available_characters_len = available_characters.size();
	std::string  s;

	while (len--) s += available_characters[rand() % available_characters_len];
	return s;
}

/**
 * @brief
 * Check the time of last activity of each clients, if that time delay is greater than TIMEOUT, send a PING and
 * set the has_been_pinged flag to true
 * If the client has been pinged and the time delay is greater than TIMEOUT_SINCE_PING, disconnect the client
 *
 * @throw disconnect() can throw ProblemWithClose() if the close() function fails.
 */
void Server::_check_time_of_last_msg(void)
{
	std::map<int, Client>::iterator const end = this->_clients_by_socket.end();

	for (std::map<int, Client>::iterator client_by_socket = this->_clients_by_socket.begin(); client_by_socket != end;
	     ++client_by_socket)
	{
		Client &client = client_by_socket->second;

		if (client.get_has_been_pinged())
		{
			if (client.time_since_last_msg() / CLOCKS_PER_SEC < TIMEOUT_SINCE_PING)
				continue;
			client.set_mode(IsAboutToBeDisconnected);
		}
		else
		{
			if (client.time_since_last_msg() / CLOCKS_PER_SEC < TIMEOUT)
				continue;
			client.set_ping_token(random_string(10));
			client.set_has_been_pinged(true);
			client.set_last_msg_time(clock());
			client.append_to_msg_out(':' + this->_name + " PING " + client.get_ping_token());
		}
	}
}
