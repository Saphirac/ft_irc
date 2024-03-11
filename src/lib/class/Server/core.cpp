/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:58:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 14:25:07 by mcourtoi         ###   ########.fr       */
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
#include <csignal>
#include <sys/epoll.h>

extern bool interrupted;

// Shared fields //

static std::string const raw_operator_hosts[] = {
	// TODO: replace this with the actual operator hosts
	"localhost",
	"127.0.0.1",
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
	std::make_pair("CAP", &Server::_cap),
	std::make_pair("MODE", &Server::_mode),
	std::make_pair("NICK", &Server::_nick),
	std::make_pair("PING", &Server::_ping),
	std::make_pair("PONG", &Server::_pong),
	std::make_pair("OPER", &Server::_oper),
	std::make_pair("PASS", &Server::_pass),
	std::make_pair("USER", &Server::_user),
	std::make_pair("QUIT", &Server::_quit),
	std::make_pair("JOIN", &Server::_join),
	std::make_pair("PART", &Server::_part),
	std::make_pair("TOPIC", &Server::_topic),
	std::make_pair("INVITE", &Server::_invite),
	std::make_pair("KICK", &Server::_kick),
	std::make_pair("PRIVMSG", &Server::_privmsg),
	std::make_pair("NOTICE", &Server::_notice),
};
Server::CommandMap const Server::_commands_by_name = CommandMap(
	_raw_commands_by_name,
	_raw_commands_by_name + sizeof(_raw_commands_by_name) / sizeof(*_raw_commands_by_name));

// Utils //

/**
 * @brief Creates a new sockaddr_in instance and initializes its fields.
 *
 * @param sin_family The value to assign to the `sockaddr_in::sin_family` field.
 * @param sin_port The value to assign to the `sockaddr_in::sin_port` field.
 * @param sin_addr The value to assign to the `sockaddr_in::sin_addr.s_addr` field.
 *
 * @return The newly created sockaddr_in instance.
 */
inline static sockaddr_in new_sockaddr_in(
	sa_family_t const sin_family,
	in_port_t const   sin_port,
	in_addr_t const   sin_addr)
{
	sockaddr_in sock_addr;

	sock_addr.sin_family = sin_family;
	sock_addr.sin_port = sin_port;
	sock_addr.sin_addr.s_addr = sin_addr;

	return sock_addr;
}

/**
 * @brief Generates the string representation of the now time.
 *
 * @return The string representation of the now time.
 *
 * @throw `ProblemWithTime` if `time()` fails.
 * @throw `ProblemWithStrftime` if `strftime()` fails.
 */
inline static std::string now_time(void)
{
	time_t const raw_time = time(NULL);

	if (raw_time == -1)
		throw ProblemWithTime();

	tm const *const time_info = localtime(&raw_time);
	char            buffer[42];

	if (!strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info))
		throw ProblemWithStrftime();

	return buffer;
}

/**
 * @brief Generates the string representation of the now date.
 *
 * @return The string representation of the now date.
 *
 * @throw `ProblemWithTime` if `time()` fails.
 * @throw `ProblemWithStrftime` if `strftime()` fails.
 */
inline static std::string now_date(void)
{
	time_t const raw_time = time(NULL);

	if (raw_time == -1)
		throw ProblemWithTime();

	tm const *const time_info = localtime(&raw_time);
	char            buffer[42];

	if (!strftime(buffer, sizeof(buffer), "%Y-%m-%d", time_info))
		throw ProblemWithStrftime();

	return buffer;
}

/**
 * @brief
 * Sets the global variable that notice the started Server instances that a SIGINT has been raised, making them stop.
 *
 * @param signal_number The number of the signal that has been raised.
 */
inline static void stop_server(int signal_number __attribute__((unused))) { interrupted = true; }

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
	_epoll_socket(epoll_create1(0)),
	_sock_addr(new_sockaddr_in(AF_INET, htons(port), INADDR_ANY)),
	_name(name),
	_password(password),
	_compilation_date(__DATE__),
	_compilation_time(__TIME__),
	_creation_date(now_date()),
	_creation_time(now_time()),
	_clients_by_socket(),
	_clients_by_nickname(),
	_channels_by_name()
{
	if (this->_socket == -1)
		throw ProblemWithSocket();

	if (this->_epoll_socket == -1)
		throw ProblemWithEpollCreate1();

	if (bind(this->_socket, (struct sockaddr *)&this->_sock_addr, sizeof(this->_sock_addr)) == -1)
		throw ProblemWithBind();

	if (listen(this->_socket, 10) == -1)
		throw ProblemWithListen();

	epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = this->_socket;

	if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, this->_socket, &event) == -1)
		throw ProblemWithEpollCtl();

	signal(SIGINT, stop_server);
}

// Destructor //

/**
 * @throw ProblemWithClose() if close() fails
 */
Server::~Server(void)
{
	int ret = 0;

	if (this->_socket != -1)
		ret = close(this->_socket);
	if (this->_epoll_socket != -1)
		ret |= close(this->_epoll_socket);
	if (ret == -1)
		throw ProblemWithClose();
}
