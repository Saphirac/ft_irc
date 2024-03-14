/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:57:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 23:39:53 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include "class/exception/ProblemWithConnect.hpp"
#include "class/exception/ProblemWithSocket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <string>
#include <sys/socket.h>

extern volatile bool bot_interrupted;

/**
 * @brief the initializer by std::pair of the CommandMap
 * 
 */
Bot::_CommandPair const Bot::_raw_commands_by_name[] = {
	std::make_pair("INVITE", &Bot::_invite),
	std::make_pair("PING", &Bot::_ping),
	std::make_pair("PRIVMSG", &Bot::_privmsg),
};

/**
 * @brief a map of all the commands that can be used by the bot
 * 
 */
Bot::_CommandMap const Bot::_commands_by_name = _CommandMap(
	_raw_commands_by_name,
	_raw_commands_by_name + sizeof(_raw_commands_by_name) / sizeof(*_raw_commands_by_name));

/**
 * @brief called if sigint (^C) is used to change the global variable bot_interrupted to true
 * 
 * @param signal_number unused argument
 */
inline static void handle_sigint(int signal_number __attribute__((unused))) { bot_interrupted = true; }

/**
 * @param port the server port
 * @param password the password to use to connect to the server
 * 
 * @throw ProblemWithSocket if the socket creation failed
 * @throw ProblemWithConnect if the connection to the server failed
 */
Bot::Bot(int const port, std::string const &password) : _socket(socket(AF_INET, SOCK_STREAM, 0)), _password(password)
{
	// Socket creation
	if (this->_socket < 0)
		throw ProblemWithSocket();

	// Preparation of server_addr
	sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Connect to server
	if (connect(this->_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0)
		throw ProblemWithConnect();

	// Set the signal handler for sigint
	signal(SIGINT, handle_sigint);
}

/**
 * @brief Destroy the Bot:: Bot object
 * 
 * @throw close() can throw a ProblemWithClose exception
 */
Bot::~Bot() { this->_disconnect(); }

