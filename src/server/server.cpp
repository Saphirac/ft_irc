/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:55:39 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/19 21:39:54 by mcourtoi         ###   ########.fr       */
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
#include <sys/epoll.h>
#include "Server.hpp"

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

void	ctrl_epoll(int epoll_fd, Server *myserver)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, myserver->getSocket(), myserver->getServerEvent()) == -1)
	{
		std::cerr << "Problem with error_ctl()." << std::endl;
		exit (EXIT_FAILURE);
	}
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

Server	*create_server(int chosen_addr, std::string password)
{
	Server *myserver = new Server(chosen_addr, password, "MyServer");

	myserver->init_server();
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
