/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:55:39 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/20 17:30:56 by mcourtoi         ###   ########.fr       */
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


