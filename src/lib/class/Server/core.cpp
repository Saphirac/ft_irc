/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:58:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/26 17:02:03 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <ctime>

// ***************************************************************************************************************** //
//                                                   Shared Fields                                                   //
// ***************************************************************************************************************** //
static std::string const raw_operator_hosts[] = {
	// TODO: replace this with the actual operator hosts
	"tmp",
};
static size_t const         raw_operator_hosts_len = sizeof(raw_operator_hosts) / sizeof(*raw_operator_hosts);
std::set<std::string> const Server::_operator_hosts =
	std::set<std::string>(raw_operator_hosts, raw_operator_hosts + raw_operator_hosts_len);

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param name The name of the server.
 * @param version The version of the server.
 * @param password The password of the server (required to connect to it).
 */
/*Server::Server(std::string const &name, std::string const &version, std::string const &password) :
	_name(name),
	_version(version),
	_password(password),
	_creation_date(),
	_creation_time(),
	_compilation_date(__DATE__),
	_compilation_time(__TIME__),
	_clients_by_socket(),
	_clients_by_nickname()
{
	time_t const raw_time = time(NULL);
	tm const    *time_info = localtime(&raw_time);
	char         buffer[80];

	strftime(buffer, sizeof(buffer), "%Y-%m-%d", time_info);
	this->_creation_date = buffer;
	strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info);
	this->_creation_time = buffer;
}*/

Server::Server(int const port, std::string const name, std::string const password, bool shutdown) :
	_port(port),
	_name(name),
	_password(password),
	_shutdown(shutdown)
{
	this->init_map_cmd();
	if (DEBUG)
		std::cout << "Server constructor called\n";
}

// ******************************************************************************************************************
// //
//                                                     Destructor //
// ******************************************************************************************************************
// //
Server::~Server(void) { this->_clients_socket.clear(); close(this->_socket); }

// ***************************************************************************************************************** //
//                                              Public Member Functions                                              //
// ***************************************************************************************************************** //
/**
 * @brief Copies a given Client instance to the list of known clients.
 *
 * @param client The Client instance to copy.
 */
/*void Server::add_client(Client *client)
{
	std::pair<std::map<int, Client *>::iterator, bool> ret =
		this->_clients_socket.insert(std::make_pair(client->get_socket(), client));

	if (ret.second)
		this->_clients_nick.insert(std::make_pair(client->get_nickname(), *ret.first->second));
}*/

/**
 * @brief Removes a client from the list of known clients.
 *
 * @param client The Client instance to remove.
 */
/*
void Server::remove_client(Client *client)
{
	this->_clients_nick.erase(client->get_nickname());
	this->_clients_socket.erase(client->get_socket());
}*/

// Getters //

int Server::get_port(void) const { return this->_port; }
int Server::get_socket(void) const { return this->_socket; }
int Server::get_epoll_socket(void) const { return this->_epoll_socket; }

std::string const &Server::get_name(void) const { return this->_name; }
std::string const &Server::get_creation_date(void) const { return this->_creation_date; }
std::string const &Server::get_creation_time(void) const { return this->_creation_time; }
std::string const &Server::get_compilation_date(void) const { return this->_compilation_date; }
std::string const &Server::get_compilation_time(void) const { return this->_compilation_time; }
std::string const &Server::get_password(void) const { return this->_password; }

struct epoll_event       *Server::get_epoll_event(void) const { return this->_epoll_event; }
struct sockaddr_in const &Server::get_sock_addr(void) const { return this->_sock_addr; }
socklen_t const          &Server::get_sock_len(void) const { return this->_sock_len; }

std::map<int, Client *> const           &Server::get_clients_socket(void) const { return this->_clients_socket; }
std::map<std::string, Client *> const &Server::get_clients_nick(void) const { return this->_clients_nick; }
std::vector<Channel *> const          &Server::get_channels(void) const { return this->_channels; }

bool Server::get_shutdown(void) const { return this->_shutdown; }

// Setters //

void Server::set_socket(int const socket) { this->_socket = socket; }
void Server::set_epoll_socket(int const epoll_socket) { this->_epoll_socket = epoll_socket; }
void Server::set_port(int const port) { this->_port = port; }

void Server::set_name(std::string const &name) { this->_name = name; }
void Server::set_creation_date(std::string const &creation_date) { this->_creation_date = creation_date; }
void Server::set_creation_time(std::string const &creation_time) { this->_creation_time = creation_time; }
void Server::set_compilation_date(std::string const &compilation_date) { this->_compilation_date = compilation_date; }
void Server::set_compilation_time(std::string const &compilation_time) { this->_compilation_time = compilation_time; }
void Server::set_password(std::string const &password) { this->_password = password; }

void Server::set_sock_addr(struct sockaddr_in const &sock_addr) { this->_sock_addr = sock_addr; }
void Server::set_sock_len() { this->_sock_len = sizeof(this->_sock_addr); }

void Server::set_clients_socket(std::map<int, Client *> const &clients_socket) { this->_clients_socket = clients_socket; }
void Server::set_clients_nick(std::map<std::string, Client *> const &clients_nick)
{
	this->_clients_nick = clients_nick;
}
void Server::set_channels(std::vector<Channel *> const &channels) { this->_channels = channels; }

void Server::set_shutdown(bool const shutdown) { this->_shutdown = shutdown; }

/**
 * @brief Create a socket object
 *
 * @throw Server::ProblemWithSocket
 */

void Server::create_and_set_socket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw Server::ProblemWithSocket();
}

/**
 * @brief this function is intended to set the epoll_event struct for the server socket
 *
 * @return struct epoll_event
 */
void Server::set_epoll_event()
{
	if (DEBUG)
		std::cout << "set_epoll_event() member function of server called\n";
	this->_epoll_event = new epoll_event;
	this->_epoll_event->events = EPOLLIN;
	this->_epoll_event->data.fd = this->_socket;
}

// Methods 

/**
 * @brief create a struct sockaddr to listen to chosen port on any addresses
 *
 * @param fd_socket
 * @return sockaddr_in&
 */
struct sockaddr_in Server::bind_assign_sockaddr()
{
	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(this->_port);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
		throw Server::ProblemWithSockAddr();
	this->_sock_addr = sock_addr;
	return sock_addr;
}

void	Server::add_client(Client *client)
{
	this->_clients_socket[client->get_socket()] = client;

	if (!client->get_nickname().empty())
		this->_clients_nick[client->get_nickname()] = client;
}

void	Server::remove_client(Client *client)
{
	this->_clients_socket.erase(client->get_socket());
	if (!client->get_nickname().empty())
		this->_clients_nick.erase(client->get_nickname());
}
