/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:55:39 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/15 16:36:53 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <sys/types.h>
#include "irc.hpp"
#include "Server.hpp"

/**
 * @brief Create a socket object
 * 
 * @todo create an exception to be thrown for more proper use
 * 
 * @return socket create / if failure exit with EXIT_FAILURE
 */

int	create_socket()
{
	int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket == -1)
	{
		std::cerr << "Failed to create socket. errno: " << errno << std::endl;
		// TODO: Change this exit with an exception
		exit (EXIT_FAILURE);
	}
	return fd_socket;
}

/**
 * @brief create a struct sockaddr to listen to chosen port on any addresses  
 * 
 * @param fd_socket 
 * @return sockaddr_in& 
 */
sockaddr_in	bind_assign_sockaddr(int fd_socket, int chosen_addr)
{
	sockaddr_in	sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(chosen_addr); 
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
	{
		std::cerr << "Failed to bind address. errno: " << errno << std::endl;
		exit (EXIT_FAILURE);
	}
	return sock_addr;
}

void	send_welcome_message(int connection)
{
	std::string response;

	// send welcome msg 01
	response = ":ircserv 001 mcourtoi :Welcome to the Internet Relay Network mcourtoi!mcourtoi@127.0.0.1\r\n";
	send(connection, response.c_str(), response.size(), 0);
	
	// send welcome msg 02
	response = ":ircserv 002 mcourtoi :Your host is ircserv, running version 1.2\r\n";
	send(connection, response.c_str(), response.size(), 0);

	// send welcome msg 03
	response = ":ircserv 003 mcourtoi :This server was created 2024/01/15\r\n";
	send(connection, response.c_str(), response.size(), 0);

	// send welcome msg 04
	response = ":ircserv 004 mcourtoi :ircserv 1.0 -none- itkol\r\n";
	send(connection, response.c_str(), response.size(), 0);
}

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

void	init_server(int chosen_addr, Server *myserver)
{
	myserver->setPort(chosen_addr);
	myserver->setSocket(create_socket());
	myserver->setSockAddr(bind_assign_sockaddr(myserver->getSocket(), myserver->getPort()));
	myserver->setSockLen();
	myserver->setName("MyServer");
}

Server	*create_server(int chosen_addr, std::string password)
{
	Server *myserver = new Server(chosen_addr, password);

	std::cout << "hello\n";
	init_server(chosen_addr, myserver);
	if (listen(myserver->getSocket(), 10) < 0) 
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	int connection = accept(myserver->getSocket(),
			(struct sockaddr*)&myserver->getSockAddr(),
			(socklen_t*)&myserver->getSockLen());

	if (connection < 0)
	{
		std::cout << "Failed to connect. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
	read_and_respond(connection);

	close(connection);
	close(myserver->getSocket());

	return myserver;
}
