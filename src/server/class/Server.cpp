/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/18 00:31:09 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "IrcMessage.hpp"
#include <cstdio>

Server::Server(int const port, std::string const name, std::string const password, bool shutdown) :
	_port(port),
	_name(name),
	_password(password),
	_shutdown(shutdown)
{
	if (DEBUG)
		std::cout << "Server constructor called\n";
}

Server::~Server()
{
	if (DEBUG)
		std::cout << "Server destructor called\n";
	close(this->_socket);
}

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

/**
 * @brief init server by creating a socket, binding it to a port and listening to it
 *
 * // TODO : throw exceptions bytes_readinstead of exit
 *
 */
void Server::init_server()
{
	this->create_and_set_socket();
	this->bind_assign_sockaddr();
	this->set_sock_len();
	if (listen(this->_socket, 10) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Init epoll //

/**
 * @brief Create an epoll object with epoll_create1()
 * in case of error, exit with EXIT_FAILURE
 * 
 * // TODO : change exit failure with exception
 *
 * @return the created epoll fd
 */
int create_epoll()
{
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		std::cerr << "Problem with epoll fd creation." << std::endl;
		exit(EXIT_FAILURE);
	}
	return epoll_fd;
}

/**
 * @brief add to epoll list of controlled fd the server socket
 *
 * TODO : change exit failure with exception
 * @param epoll_fd
 * @param myserver
 */
void Server::ctrl_epoll_add(int epoll_fd, int socket, struct epoll_event *e_event)
{
	printf("epoll_fd : %d\n socket : %d\n", epoll_fd, socket);
	printf("epoll event adress : %p\n", e_event);

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, e_event) == -1)
	{
		std::cerr << "errno : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void control_socket(int const socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << "Erreur lors de la récupération des flags du socket." << std::endl;
	}
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) == -1)
	{
		std::cerr << "Erreur lors du passage du socket en mode non bloquant." << std::endl;
	}
}

// Init server //

void Server::handle_client_event(Client *client)
{
	char buffer[513];
	int  bytes_read;

	bytes_read = recv(client->get_socket(), buffer, 512, 0);
	if (bytes_read == -1)
	{
		std::cerr << "Problem with recv()." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bytes_read == 0)
	{
		std::cout << "Client disconnected." << std::endl;
		close(client->get_socket());
		return;
	}
	buffer[bytes_read] = '\0';
	IrcMessage test = parse_irc_message(std::string(buffer));
	test.display();
	if (test.get_command() == "CAP")
		this->cap(*client, *test.get_params());
	else if (test.get_command() == "JOIN")
		this->join(*client, *test.get_params());
}

/**
 * @brief add new client to the list of clients
 *
 * // TODO : change exit failure with exception
 * // TODO : use a client with a handshake bool to check if nickname and other received input is correct
 *
 */
void Server::handle_new_connection()
{
	int	const client_socket = accept(this->_socket, NULL, NULL);
	if (client_socket == -1)
	{
		std::cerr << "Problem with accept()." << std::endl;
		exit(EXIT_FAILURE);
	}
	control_socket(client_socket);
	this->_clients_socket[client_socket] = new Client(client_socket, "", "", "", "",  0);
	this->_clients_socket[client_socket]->set_epoll_event();
	printf("epoll_fd : %d\n client_socket : %d\n", this->_epoll_socket, client_socket);
	printf("epoll event adress : %p\n", this->_clients_socket[client_socket]->get_epoll_event());
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients_socket[client_socket]->get_epoll_event());
}

/**
 * @brief create a loop to manage new incoming connections or messages
 *
 * @param myserver
 */
void Server::epoll_loop()
{
	int                fds_ready;
	struct epoll_event events[MAX_CLIENTS];

	fds_ready = epoll_wait(this->_epoll_socket, events, MAX_CLIENTS, -1);
	if (fds_ready == -1)
	{
		std::cerr << "Problem with epoll_wait() : " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < fds_ready; i++)
	{
		if (events[i].data.fd == this->_socket)
			handle_new_connection();
		else
			handle_client_event(this->_clients_socket[events[i].data.fd]);
	}
}

void Server::create_server()
{
	this->_epoll_socket = create_epoll();
	printf("epoll_socket : %d\n", this->_epoll_socket);
	this->set_epoll_event();
	ctrl_epoll_add(this->_epoll_socket, this->_socket, this->_epoll_event);
	while (this->_shutdown == false)
	{
		epoll_loop();
	}
	// read_and_respond(connection);
}
