/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 22:06:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 07:24:30 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef DEBUG
#	include <iostream>
#endif

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
#define INACTIVITY_TIMEOUT 60
// How many seconds since the last PING sent to a client the server will wait without receiving a PONG from that client
// before disconnecting that client.
#define PING_TIMEOUT 15
// The number of clients that the server can accept (chosen arbitrarily).
#define MAXIMUM_NUMBER_OF_CLIENTS 7

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
			this->_check_time_of_last_msg(client);
			client.send_msg_out();
			++it;
			if (client.has_mode(IsAboutToBeDisconnected))
				this->_remove_client(client);
		}
	}
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
		{
			if (this->_clients_by_socket.size() < MAXIMUM_NUMBER_OF_CLIENTS)
				this->_new_client_connection();
		}
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
		client.set_last_msg_time(time(NULL));
	if (bytes_received == 0)
		return this->_remove_client(client);

#ifdef DEBUG
	std::string buffer_as_string(buffer, bytes_received);

	while (buffer_as_string.find("\r\n") != std::string::npos)
		buffer_as_string.replace(buffer_as_string.find("\r\n"), 2, "\\r\\n");
	std::cout << "Received [" << buffer_as_string << "] from " << client.get_nickname() << '\n';
#endif
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
		Message const                     msg(raw_msg);
		std::string const                &command_name = msg.get_command();
		_CommandMap::const_iterator const command_by_name = this->_commands_by_name.find(command_name);

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
 * Checks whether a client connection is still alive by checking the time since the last message was received,
 * and sending a PING if necessary.
 *
 * @param client The client for which the connection will be checked.
 *
 * @throw `ProblemWithClose` if `close()` fails.
 */
void Server::_check_time_of_last_msg(Client &client) const
{
	time_t const time_since_last_msg = client.time_since_last_msg();

	if (client.get_has_been_pinged())
	{
		if (time_since_last_msg > PING_TIMEOUT)
			client.set_mode(IsAboutToBeDisconnected);
		return;
	}

	if (time_since_last_msg < INACTIVITY_TIMEOUT)
		return;
	client.set_ping_token(random_string(10));
	client.set_has_been_pinged(true);
	client.set_last_msg_time(time(NULL));
	client.append_to_msg_out(':' + this->_name + " PING " + client.get_ping_token());
}

/**
 * @brief
 * Makes a client leave all their joined channels using PART,
 * close their connection, and removes them from the list of known clients.
 *
 * @param client The client to remove.
 * @param part_text The PART text to send to the channels in which the user is joined.
 *
 * @throw `ProblemWithClose` if `close()` fails.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_remove_client(Client &client, std::string const &part_text)
{
	this->_make_client_leave_all_their_joined_channels(client, part_text);
	this->_clients_by_nickname.erase(client.get_nickname());
	this->_clients_by_socket.erase(client.get_socket());
}

/**
 * @brief Sends the welcome message to a client that registered.
 *
 * @param client The client that registered.
 */
void Server::_welcome(Client &client) const
{
	std::string const user_mask = client.user_mask();

	client.append_formatted_reply_to_msg_out(RPL_WELCOME, &user_mask);
	client.append_formatted_reply_to_msg_out(RPL_YOURHOST, &this->_name, &this->_version);
	client.append_formatted_reply_to_msg_out(RPL_CREATED, &this->_creation_date);
	client.append_formatted_reply_to_msg_out(RPL_MYINFO, &this->_name, &this->_version, USER_MODES, CHANNEL_MODES);
}

/**
 * @brief Makes a user leave all their joined channels using the PART command.
 *
 * @param user The user that will leave all their joined channels.
 * @param part_text The PART text to send to the channels in which the user is joined.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_make_client_leave_all_their_joined_channels(Client &client, std::string const &part_text)
{
	Client::JoinedChannelMap const &joined_channels_by_name = client.get_joined_channels_by_name();

	if (joined_channels_by_name.empty())
		return;

	Client::JoinedChannelMap::const_iterator joined_channel_by_name = joined_channels_by_name.begin();
	std::string                              channels_to_leave = joined_channel_by_name->first;

	while (++joined_channel_by_name != joined_channels_by_name.end())
		channels_to_leave += ',' + joined_channel_by_name->first;

	CommandParameterVector part_arguments(2);

	part_arguments[0] = channels_to_leave;
	part_arguments[1] = part_text;
	this->_part(client, part_arguments);
}
