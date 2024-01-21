/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 03:42:54 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int const port, std::string const password, std::string const name) : 
_port(port),
_password(password),
_name(name)
{
	if (DEBUG)
		std::cout << "Server constructor called\n";
	this->init_server();
	this->create_server();
}

Server::~Server()
{
	if (DEBUG)
		std::cout << "Server destructor called\n";
	//close(connection);
	close(this->_socket);
}

// Getters //

std::string const	&Server::getName() const
{
	if (DEBUG)
		std::cout << "getName() member function of server called\n";
	return this->_name;
}

int	Server::getIp() const
{
	if (DEBUG)
		std::cout << "getIp() member function of server called\n";
	return this->_ip;
}

int	Server::getPort() const
{
	if (DEBUG)
		std::cout << "getPort() member function of server called\n";
	return this->_port;
}

int	Server::getSocket() const
{
	if (DEBUG)
		std::cout << "getSocket() member function of server called\n";
	return this->_socket;
}

sockaddr_in const	&Server::getSockAddr() const
{
	if (DEBUG)
		std::cout << "getSockAddr() member function of server called\n";
	return this->_sock_addr;
}

socklen_t const	&Server::getSockLen() const
{
	if (DEBUG)
		std::cout << "getSockLen() member function of server called\n";
	return this->_addr_len;
}

std::string const	&Server::getPassword() const
{
	if (DEBUG)
		std::cout << "getPassword() member function of server called\n";
	return this->_password;
}

std::vector<Client *> const	&Server::getClients() const
{
	if (DEBUG)
		std::cout << "getClients() member function of server called\n";
	return this->_clients;
}

std::vector<Channel *> const	&Server::getChannels() const
{
	if (DEBUG)
		std::cout << "getChannels() member function of server called\n";
	return this->_channels;
}

int	Server::getEpollSocket() const
{
	if (DEBUG)
		std::cout << "getEpollSocket() member function of server called\n";
	return this->_epoll_socket;
}

struct epoll_event	*Server::getEpollEvent() const
{
	if (DEBUG)
		std::cout << "getEpollEvent() member function of server called\n";
	return this->_epoll_event;
}

// Setters //

void	Server::setName(std::string const &name)
{
	if (DEBUG)
		std::cout << "setName() member function of server called\n";
	this->_name = name;
}

void	Server::setIp(int const ip)
{
	if (DEBUG)
		std::cout << "getChannels() member function of server called\n";
	this->_ip = ip;
}

void	Server::setPort(int const port)
{
	if (DEBUG)
		std::cout << "setPort() member function of server called\n";
	this->_port = port;
}

void	Server::setSocket(int const socket)
{
	if (DEBUG)
		std::cout << "setSocket() member function of server called\n";
	this->_socket = socket;
}

void	Server::setSockAddr(sockaddr_in const &addr)
{
	if (DEBUG)
		std::cout << "setSockAddr() member function of server called\n";
	this->_sock_addr = addr;
}

void	Server::setSockLen()
{
	if (DEBUG)
		std::cout << "setSockLen() member function of server called\n";
	this->_addr_len = sizeof(this->_sock_addr);
}

void	Server::setPassword(std::string const &password)
{
	if (DEBUG)
		std::cout << "setPassword() member function of server called\n";
	this->_password = password;
}

void	Server::setClients(std::vector<Client*> const &clients)
{
	if (DEBUG)
		std::cout << "setClients() member function of server called\n";
	this->_clients = clients;
}

void	Server::setChannels(std::vector<Channel*> const &channels)
{
	if (DEBUG)
		std::cout << "setChannels() member function of server called\n";
	this->_channels = channels;
}

void	Server::setEpollSocket(int fd)
{
	if (DEBUG)
		std::cout << "setEpollSocket() member function of server called\n";
	this->_epoll_socket = fd;
}

/**
 * @brief this function is intended to set the epoll_event struct for the server socket
 * 
 * @return struct epoll_event 
 */
struct epoll_event	Server::setEpollEvent()
{
	if (DEBUG)
		std::cout << "getEpollEvent() member function of server called\n";
	this->_epoll_event->events = EPOLLIN;
	this->_epoll_event->data.fd = this->_socket;
}

/**
 * @brief Create a socket object
 * 
 * @throw Server::ProblemWithSocket
 */

void	Server::create_and_set_socket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw Server::ProblemWithSocket();
}

/**
 * @brief create a struct sockaddr to listen to chosen port on any addresses  
 * 
 * @param fd_socket 
 * @return sockaddr_in& 
 */
sockaddr_in	Server::bind_assign_sockaddr()
{
	sockaddr_in	sock_addr;
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
 * // TODO : throw exceptions instead of exit
 * 
 */
void	Server::init_server()
{
	this->create_and_set_socket();
	this->bind_assign_sockaddr();
	this->setSockLen();
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
 * @return the created epoll fd
 */
int	create_epoll()
{
	int	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{	
		std::cerr << "Problem with epoll fd creation." << std::endl;
		exit (EXIT_FAILURE);
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
void	Server::ctrl_epoll_add(int epoll_fd, int socket, struct epoll_event *e_event)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, e_event) == -1)
	{
		std::cerr << "Problem with error_ctl()." << std::endl;
		exit (EXIT_FAILURE);
	}
}

// Init Server //

/**
 * @brief add new client to the list of clients
 * 
 * // TODO : change exit failure with exception
 * // TODO : use a client with a handshake bool to check if nickname and other received input is correct
 * 
 */
void	Server::handle_new_connection()
{
	int	client_socket;
	struct sockaddr_in	client_addr;
	socklen_t	client_addr_len = sizeof(client_addr);

	client_socket = accept(this->_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_socket == -1)
	{
		std::cerr << "Problem with accept()." << std::endl;
		exit (EXIT_FAILURE);
	}
	
	this->_clients->push_back(new Client(client_socket, client_addr));
	this->_clients->back().getConnectionInfo(this);
	if (this->_clients->back().getIsComplete() == false)
	{
		std::cerr << "Client connection incomplete." << std::endl;
		return;
	}
	
	this->_client_socket[client_socket] = this->_clients.back();
	this->_clients.back()->setEvent();
	ctrl_epoll_add(this->_epoll_socket, client_socket, this->_clients.back()->getEvent());
	this->send_welcome_message(client_socket);
}

void	Server::handle_client_event(Client *client)
{
	std::string	buffer;
	int		bytes_read;

	bytes_read = recv(client->getSocket(), buffer, 512, 0);
	if (bytes_read == -1)
	{
		std::cerr << "Problem with recv()." << std::endl;
		exit (EXIT_FAILURE);
	}
	if (bytes_read == 0)
	{
		std::cout << "Client disconnected." << std::endl;
		close(client->getSocket());
		return;
	}
	buffer[bytes_read] = '\0';
	std::cout << "Received " << bytes_read << " bytes from client." << std::endl;
	parse_command(buffer, client);
	client->getCmd().execCommand();
}

/**
 * @brief create a loop to manage new incoming connections or messages
 * 
 * @param myserver 
 */
void	Server::epoll_loop()
{
	int	fds_ready;
	struct epoll_event	events[MAX_CLIENTS];

	fds_ready = epoll_wait(this->_socket, events, MAX_CLIENTS, -1);
	if (fds_ready == -1)
	{
		std::cerr << "Problem with epoll_wait()." << std::endl;
		exit (EXIT_FAILURE);
	}

	for (int i = 0; i < fds_ready; i++)
	{
		if (events[i].data.fd = this->_socket)
			handle_new_connection(); 
		else
			handle_client_event(this->_client_socket[events[i].data.fd]);
	}
}

void	Server::create_server()
{
	this->_epoll_socket = create_epoll();
	this->setEpollEvent();
	ctrl_epoll_add(this->_epoll_socket, this->_socket, this->_epoll_event);
	
	//read_and_respond(connection);
}
