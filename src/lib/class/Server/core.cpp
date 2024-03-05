/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:58:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:36:53 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "class/exception/ProblemWithBind.hpp"
#include "class/exception/ProblemWithClose.hpp"
#include "class/exception/ProblemWithEpollCreate1.hpp"
#include "class/exception/ProblemWithEpollCtl.hpp"
#include "class/exception/ProblemWithListen.hpp"
#include "class/exception/ProblemWithSocket.hpp"
#include "class/exception/ProblemWithStrftime.hpp"
#include "class/exception/ProblemWithTime.hpp"
#include <ctime>
#include <iostream>
#include <unistd.h>

// Shared fields //

static std::string const raw_operator_hosts[] = {
	// TODO: replace this with the actual operator hosts
	"tmp",
};
std::set<std::string> const Server::_operator_hosts = std::set<std::string>(
	raw_operator_hosts,
	raw_operator_hosts + sizeof(raw_operator_hosts) / sizeof(*raw_operator_hosts));

static std::pair<std::string const, std::string const> const raw_operator_ids[] = {
	std::make_pair("jodufour", "eagle"),
	std::make_pair("mcourtoi", "black panther"),
	std::make_pair("gle-mini", "tiger"),
};
std::map<std::string, std::string const> const Server::_operator_ids = std::map<std::string, std::string const>(
	raw_operator_ids,
	raw_operator_ids + sizeof(raw_operator_ids) / sizeof(*raw_operator_ids));

Server::CommandPair const Server::_raw_commands_by_name[] = {
	// TODO: add missing commands
	std::make_pair("AWAY", &Server::_away),
	std::make_pair("MODE", &Server::_mode),
	std::make_pair("NICK", &Server::_nick),
	std::make_pair("OPER", &Server::_oper),
	std::make_pair("PASS", &Server::_pass),
	std::make_pair("QUIT", &Server::_quit),
	std::make_pair("USER", &Server::_user),
};
Server::CommandMap const Server::_commands_by_name = CommandMap(
	_raw_commands_by_name,
	_raw_commands_by_name + sizeof(_raw_commands_by_name) / sizeof(*_raw_commands_by_name));

// Constructors //

/**
 * @param port The port on which the server will listen.
 * @param name The name of the server.
 * @param version The version of the server.
 * @param password The password of the server (required to connect to it).
 *
 * @throw `ProblemWithSocket` if `socket()` fails.
 * @throw `ProblemWithBind` if `bind()` fails
 * @throw `ProblemWithListen` if `listen()` fails.
 * @throw `ProblemWithEpollCreate1` if `epoll_create1()` fails.
 * @throw `ProblemWithEpollCtl` if `epoll_ctl()` fails.
 * @throw `ProblemWithTime` if `time()` fails.
 * @throw `ProblemWithStrftime` if `strftime()` fails.
 */
Server::Server(int const port, std::string const &name, std::string const &password) :
	_socket(socket(AF_INET, SOCK_STREAM, 0)),
	_sock_addr(),
	_sock_len(sizeof(this->_sock_addr)),
	_epoll_socket(epoll_create1(0)),
	_shutdown(true),
	_name(name),
	_password(password),
	_creation_date(),
	_creation_time(),
	_compilation_date(__DATE__),
	_compilation_time(__TIME__),
	_clients_by_socket(),
	_clients_by_nickname(),
	_channels_by_name()
{
	if (this->_socket == -1)
		throw ProblemWithSocket();

	this->_sock_addr.sin_family = AF_INET;
	this->_sock_addr.sin_port = htons(port);
	this->_sock_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_socket, (struct sockaddr *)&this->_sock_addr, sizeof(this->_sock_addr)) == -1)
		throw ProblemWithBind();

	if (listen(this->_socket, 10) == -1)
		throw ProblemWithListen();

	if (this->_epoll_socket == -1)
		throw ProblemWithEpollCreate1();

	epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = this->_socket;

	if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, this->_socket, &event) == -1)
		throw ProblemWithEpollCtl();

	time_t const raw_time = time(NULL);

	if (raw_time == -1)
		throw ProblemWithTime();

	tm const *const time_info = localtime(&raw_time);
	char            buffer[42];

	if (!strftime(buffer, sizeof(buffer), "%Y-%m-%d", time_info))
		throw ProblemWithStrftime();

	this->_creation_date = buffer;

	if (!strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info))
		throw ProblemWithStrftime();

	this->_creation_time = buffer;

	// TODO: take a signal number in parameters to set a signal handler to be able to shutdown the server properly
}

// Destructor //

/**
 * @throw ProblemWithClose() if close() fails
 */
Server::~Server(void)
{
	this->_clients_by_socket.clear();
	if (this->_socket != -1 && close(this->_socket) == -1)
		throw ProblemWithClose();

	// TODO: close the epoll socket
}
