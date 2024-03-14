/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 19:01:27 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 22:32:37 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include "class/exception/ProblemWithRecv.hpp"
#include "class/exception/ProblemWithSend.hpp"
#include "class/exception/ProblemWithSelect.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096
#define TIMEOUT 5

/**
 * @brief the global variable to stop the bot
 * 
 */
volatile bool bot_interrupted = false;

/**
 * @brief removes the crlf from a string
 * 
 * @param str the string to remove the crlf from
 * @return std::string a new string without the crlf
 */
inline static std::string remove_crlf(std::string const &str)
{
	std::string result = str;

	if (result.length() >= 2 && result[result.length() - 2] == '\r' && result[result.length() - 1] == '\n')
		result.erase(result.length() - 2);
	return result;
}

/**
 * @brief Sends the PASS NICK USER and MODE commands to the server
 * 
 */
void Bot::_send_connexion_message()
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
inline static bool is_channel(std::string const &str) { return str[0] == '#' || str[0] == '&' || str[0] == '+' || str[0] == '!'; }

/**
 * @brief Responds to a PRIVMSG message with bar if the second parameter is foo and Wall-E if the second parameter is Eve
 * If the sender is a channel, sends the response to the channel, otherwise sends the response to the sender
 * 
 * @param msg the received message
 */
void Bot::_privmsg(Message const &msg)
{
	std::string response;

	if (msg.get_parameters().size() < 2)
		return;

	std::string const sender = is_channel(first_param) ? first_param : msg.get_prefix().who_is_sender();

	if (!second_param.empty())
	{
		if (second_param == "foo")
			response = "PRIVMSG " + sender + " bar\r\n";
		else if (second_param == "Eve")
			response = "PRIVMSG " + sender + " Wall-E\r\n";
		if (send(this->_socket, response.c_str(), response.size(), 0) < 0)
			throw ProblemWithSend();
	}
}

/**
 * @brief Responds to a received ping with a pong and the same parameter
 * 
 * @param msg the received message
 */
void Bot::_ping(Message const &msg)
{
	std::string const response = "PONG " + msg.get_parameters()[0] + "\r\n";

	if (send(this->_socket, response.c_str(), response.size(), 0) < 0)
		throw ProblemWithSend();
}

/**
 * @brief Responds to an invite by joining the channel
 * 
 * @param msg the received message
 */
void Bot::_invite(Message const &msg)
{
	std::string const response = "JOIN " + msg.get_parameters()[1] + "\r\n";
	
	if (send(this->_socket, response.c_str(), response.size(), 0) < 0)
		throw ProblemWithSend();
}

/**
 * @brief The routine that check if the bot has received a message and calls the corresponding function
 * if the command is invite / privmsg or ping
 * 
 * @param read_fds the fd that select checks
 * @param max_fd the surveiled fd with the highest-number (here we only have one)
 * @param timeout a timeval struct to indicates when to stop select
 */
void Bot::_bot_routine(fd_set &read_fds, int &max_fd, timeval &timeout)
{
	int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

	if (activity < 0)
		throw ProblemWithSelect();
	if (activity && FD_ISSET(this->_socket, &read_fds))
	{
		char    buffer[MAX_BUFFER_SIZE];
		ssize_t received = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);

		if (received < 0)
			throw ProblemWithRecv();
		else if (received == 0)
		{
			std::cout << "Server closed connection" << std::endl;
			bot_interrupted = true;
			return;
		}
		else
		{
			Message               msg(remove_crlf(std::string(buffer, received)));
			CommandIterator const command_by_name = this->_commands_by_name.find(msg.get_command());

			if (command_by_name == this->_commands_by_name.end())
				return ;
			else
				(this->*(command_by_name->second))(msg);
		}
	}
}

/**
 * @brief a simple method to close the socket, is called at the destruction of the bot
 * 
 */
void Bot::_disconnect()
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
void Bot::run()
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
