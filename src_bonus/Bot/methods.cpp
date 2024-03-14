/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 19:01:27 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 15:20:37 by mcourtoi         ###   ########.fr       */
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

volatile bool bot_interrupted = false;

inline static std::string remove_crlf(std::string const &str)
{
	std::string result = str;

	// Vérifie si la chaîne se termine par CRLF et la réduit en conséquence
	if (result.length() >= 2 && result[result.length() - 2] == '\r' && result[result.length() - 1] == '\n')
		result.erase(result.length() - 2);
	return result;
}

void Bot::_send_connexion_message()
{
	std::string const connexion_msg = "PASS " + this->_password + "\r\n" + "NICK Wall-E\r\n"
	                                + "USER userbot 0 localhost userbot\r\n" + "MODE Wall-E +B\r\n";

	if (send(this->_socket, connexion_msg.c_str(), connexion_msg.size(), 0) < 0)
		throw ProblemWithSend();
}

inline static bool is_channel(std::string const &s) { return s[0] == '#' || s[0] == '&' || s[0] == '+' || s[0] == '!'; }

// TODO : make a lookup table for these functions
void Bot::_privmsg_cmd(std::string &response, Message const &msg)
{
	if (msg.get_parameters().size() < 2)
		return;

	std::string const first_param = remove_crlf(msg.get_parameters()[0]);
	std::string const second_param = remove_crlf(msg.get_parameters()[1]);
	std::string const sender = is_channel(first_param) ? first_param : msg.get_prefix().who_is_sender();

	if (!second_param.empty())
	{
		if (second_param == "foo")
			response = "PRIVMSG " + sender + " bar\r\n";
		else if (second_param == "Eve")
			response = "PRIVMSG " + sender + " Wall-E\r\n";
	}
}

void Bot::_ping_cmd(std::string &response, Message const &msg)
{
	std::string const scd_param = remove_crlf(msg.get_parameters()[1]);

	response = "PONG " + msg.get_parameters()[0];
}

void Bot::_join_cmd(std::string &response, Message const &msg)
{
	std::cout << "Joining [" << msg.get_parameters()[1] << "]" << std::endl;
	response = "JOIN " + msg.get_parameters()[1];
}

void Bot::_bot_routine(fd_set &read_fds, int &max_fd)
{
	struct timeval timeout;
	timeout.tv_sec = 5; // Timeout après 5 secondes
	timeout.tv_usec = 0;

	int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

	if (activity < 0)
		throw ProblemWithSelect();
	
	if (activity && FD_ISSET(this->_socket, &read_fds))
	{
		char    buffer[MAX_BUFFER_SIZE];
		ssize_t received = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
		buffer[received] = '\0'; // Assurez-vous que la chaîne est correctement terminée.

		if (received < 0)
			throw ProblemWithRecv();
		else if (received == 0)
		{
			std::cout << "Server closed connection" << std::endl;
			return;
		}
		else
		{
			Message     msg((std::string(buffer, received)));
			std::string response;

			std::cout << "Command : [" << msg.get_command() << "]" << std::endl;

			if (msg.get_command() == "PRIVMSG")
				this->_privmsg_cmd(response, msg);
			if (msg.get_command() == "PING")
				this->_ping_cmd(response, msg);
			if (msg.get_command() == "INVITE")
				this->_join_cmd(response, msg);
			if (!response.empty() && send(this->_socket, response.c_str(), response.size(), 0) < 0)
				throw ProblemWithSend();
		}
	}
}

void Bot::_disconnect()
{
	if (this->_socket != -1)
		close(this->_socket);
}

void Bot::run()
{
	fd_set read_fds;
	int max_fd = this->_socket;

	this->_send_connexion_message();
	while (!bot_interrupted)
	{
		FD_ZERO(&read_fds);
		FD_SET(this->_socket, &read_fds);
		this->_bot_routine(read_fds, max_fd);
	}
}
