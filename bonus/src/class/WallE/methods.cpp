/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 19:01:27 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/15 05:39:02 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/WallE.hpp"
#include "class/exception/ProblemWithRecv.hpp"
#include "class/exception/ProblemWithSelect.hpp"
#include "class/exception/ProblemWithSend.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define TIMEOUT     1

/**
 * @brief the global variable to stop the bot
 *
 */
volatile bool bot_interrupted = false;

/**
 * @brief Appends a string to the input buffer of the Client instance.
 *
 * @param s The string to append.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void WallE::_append_to_msg_in(std::string const &s) { this->_msg_in += s; }

#define TERMINATING_SEQUENCE        "\r\n"
#define TERMINATING_SEQUENCE_LENGTH 2
#define MAXIMUM_LENGTH_FOR_MESSAGE  512

/**
 * @brief Get the first message in the input buffer of the Client instance.
 * A message is <= 512 characters and is suffixed with a CRLF sequence.
 * Then it extracts and return this message and erase it from the reste of the buffer.
 *
 * @return The message found or an empty string if either no message is found or the message is too long.
 */
std::string WallE::_get_next_msg(void)
{
	size_t const pos = this->_msg_in.find(TERMINATING_SEQUENCE);

	if (pos == std::string::npos)
		return std::string();

	std::string const msg = this->_msg_in.substr(0, pos);

	this->_msg_in.erase(0, pos + TERMINATING_SEQUENCE_LENGTH);
	if (pos > MAXIMUM_LENGTH_FOR_MESSAGE)
		return std::string();

	return msg;
}

/**
 * @brief Sends the PASS NICK USER and MODE commands to the server
 *
 */
void WallE::_send_connexion_message()
{
	std::string const connexion_msg = "PASS " + this->_password + "\r\n" + "NICK Wall-E\r\n"
	                                + "USER userbot 0 localhost userbot\r\n" + "MODE Wall-E +B\r\n";

	if (send(this->_socket, connexion_msg.c_str(), connexion_msg.size(), 0) < 0)
		throw ProblemWithSend();
}

/**
 * @brief Check if a given string corresponds to a channel (begins by #!+&)
 *
 * @param s the string to check
 * @return true if channel
 * @return false if not channel
 */
inline static bool is_channel(std::string const &str) { return std::string("#&+!").find(str[0]) != std::string::npos; }

/**
 * @brief Responds to a PRIVMSG message with bar if the second parameter is foo and Wall-E if the second parameter is
 * Eve If the sender is a channel, sends the response to the channel, otherwise sends the response to the sender
 *
 * @param msg the received message
 */
void WallE::_privmsg(Message const &msg)
{
	if (msg.get_parameters().size() < 2)
		return;

	std::string const second_param = msg.get_parameters()[1];

	if (!second_param.empty())
	{
		std::string       response;
		std::string const first_param = msg.get_parameters()[0];
		std::string const sender = is_channel(first_param) ? first_param : msg.get_prefix().who_is_sender();

		if (second_param == "foo")
			response = "PRIVMSG " + sender + " bar\r\n";
		else if (second_param == "Eve")
			response = "PRIVMSG " + sender + " Wall-E\r\n";
		if (!response.empty() && send(this->_socket, response.c_str(), response.size(), 0) < 0)
			throw ProblemWithSend();
	}
}

/**
 * @brief Responds to a received ping with a pong and the same parameter
 *
 * @param msg the received message
 */
void WallE::_ping(Message const &msg)
{
	if (msg.get_parameters().empty())
		return;

	std::string const response = "PONG " + msg.get_parameters()[0] + "\r\n";

	if (send(this->_socket, response.c_str(), response.size(), 0) < 0)
		throw ProblemWithSend();
}

/**
 * @brief Responds to an invite by joining the channel
 *
 * @param msg the received message
 */
void WallE::_invite(Message const &msg)
{
	if (msg.get_parameters().size() < 2)
		return;

	std::string const response = "JOIN " + msg.get_parameters()[1] + "\r\n";

	if (send(this->_socket, response.c_str(), response.size(), 0) < 0)
		throw ProblemWithSend();
}

/**
 * @brief handle PASSWDMISMATCH reply
 *
 */
void WallE::_pass(Message const &msg __attribute__((unused))) { bot_interrupted = true; }

/**
 * @brief The routine that check if the bot has received a message and calls the corresponding function
 * if the command is invite / privmsg or ping
 *
 * @param read_fds the fd that select checks
 * @param max_fd the surveiled fd with the highest-number (here we only have one)
 * @param timeout a timeval struct to indicates when to stop select
 */
void WallE::_bot_routine(fd_set &read_fds, int &max_fd, timeval &timeout)
{
	int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

	if (activity < 0)
		throw ProblemWithSelect();
	if (activity && FD_ISSET(this->_socket, &read_fds))
	{
		char          buffer[BUFFER_SIZE];
		ssize_t const bytes_received = recv(this->_socket, buffer, BUFFER_SIZE, 0);

		if (bytes_received < 0)
			throw ProblemWithRecv();
		else if (bytes_received == 0)
		{
			std::cout << "Server closed connection" << std::endl;
			bot_interrupted = true;
			return;
		}
		this->_append_to_msg_in(std::string(buffer, bytes_received));
	}

	if (this->_msg_in.empty())
		return;

	std::string raw_msg;

	while (!(raw_msg = this->_get_next_msg()).empty())
	{
		Message const                     msg(raw_msg);
		_CommandMap::const_iterator const command_by_name = this->_commands_by_name.find(msg.get_command());

		if (command_by_name == this->_commands_by_name.end())
			return;
		(this->*(command_by_name->second))(msg);
	}
}

/**
 * @brief a simple method to close the socket, is called at the destruction of the bot
 *
 */
void WallE::_disconnect()
{
	if (this->_socket != -1)
		close(this->_socket);
}

/**
 * @brief The main function of the bot, it sends the connexion message and then calls the bot_routine
 * Stay running until bot_interrupted is true
 * Also sets the select variables
 *
 */
void WallE::run()
{
	fd_set         read_fds;
	int            max_fd = this->_socket;
	struct timeval timeout;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0; // Timeval needs to be initalized

	this->_send_connexion_message();

	while (!bot_interrupted)
	{
		FD_ZERO(&read_fds);
		FD_SET(this->_socket, &read_fds);
		this->_bot_routine(read_fds, max_fd, timeout);
	}
}
