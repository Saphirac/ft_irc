/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:00:12 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/11/28 18:12:18 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int const port, std::string const password) : 
_port(port),
_password(password)
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

int const	Server::getSocket() const
{
	if (DEBUG)
		std::cout << "getSocket() member function of server called\n";
	return this->_socket;
}

sockaddr_in	&Server::getSockAddr() const
{
	if (DEBUG)
		std::cout << "getSockAddr() member function of server called\n";
	return this->sock_addr;
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

// Setters //

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
	this->sock_addr = addr;
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
