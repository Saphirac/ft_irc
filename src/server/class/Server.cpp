/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/20 17:33:58 by mcourtoi         ###   ########.fr       */
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
}

Server::~Server()
{
	if (DEBUG)
		std::cout << "Server destructor called\n";
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
 * @todo create an exception to be thrown for more proper use
 * 
 * @return socket create / if failure exit with EXIT_FAILURE
 */

int	Server::create_and_set_socket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket == -1)
		throw Server::ProblemWithSocket();
	return fd_socket;
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

void	Server::init_server()
{
	this->create_and_set_socket();
	this->bind_assign_sockaddr();
	this->setSockLen();
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
void	Server::ctrl_epoll(int epoll_fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_socket, this->_epoll_event) == -1)
	{
		std::cerr << "Problem with error_ctl()." << std::endl;
		exit (EXIT_FAILURE);
	}
}

// Init Server //

void	read_and_respond(int connection)
{
	std::string buffer(100, '\0');

	while (buffer != "quit\n")
	{
		ssize_t bytes_read = read(connection, &buffer[0], buffer.size() - 1);
		if (bytes_read <= 0)
		{
			if (bytes_read == 0)
			{
				// connexion fermée par le client
				std::cout << "Client disconnected." << std::endl;
			}
			else {
				// erreur de lecture
				std::cerr << "Read failed. errno: " << errno << std::endl;
			}
			return;
		}

		buffer[bytes_read] = '\0'; // Assurer la terminaison de la chaîne
		buffer = trim(buffer);
		if (buffer == "quit")
			return;
		std::cout << buffer << '\n';
		send_welcome_message(connection);
		
		buffer.assign(100, '\0');
	}
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
			handle_client_event(myserver->getClients()[events[i].data.fd]);
	}
}

Server	*create_server(int chosen_addr, std::string password)
{
	Server *myserver = new Server(chosen_addr, password, "MyServer");

	myserver->init_server();
	if (listen(myserver->getSocket(), 10) < 0) 
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	myserver->setEpollSocket(create_epoll());
	myserver->setEpollEvent();
	ctrl_epoll(myserver->getEpollEvent(), myserver);
	
	
	read_and_respond(connection);

	close(connection);
	close(myserver->getSocket());

	return myserver;
}
