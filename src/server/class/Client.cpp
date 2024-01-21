/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/21 21:06:20 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

Client::Client(int const socket, sockaddr_in sock_addr, int const name, int const nickname) :
_socket(socket),
_name(name),
_nickname(nickname),
_sock_addr(sock_addr),
_addr_len(sizeof(sockaddr_in))
{
	if (DEBUG)
		std::cout << "Client constructor called\n";
}

Client::~Client()
{
	if (DEBUG)
		std::cout << "Client destructor called\n";
}

// Getters //

int	Client::getSocket(void) const
{
	if (DEBUG)
		std::cout << "getSocket() member function of client called\n";
	return this->_socket;
}

std::string	Client::getName(void) const
{
	if (DEBUG)
		std::cout << "getName() member function of client called\n";
	return this->_name;
}

std::string	Client::getNickname(void) const
{
	if (DEBUG)
		std::cout << "getNickname() member function of client called\n";
	return this->_nickname;
}

struct epoll_event	*Client::getEvent(void) const
{
	if (DEBUG)
		std::cout << "getEvent() member function of client called\n";
	return this->_client_event;
}

int Client::getSockAddr(void) const
{
	if (DEBUG)
		std::cout << "getSockAddr() member function of client called\n";
	return this->_sock_addr;
}

// Setters //

void	Client::setSocket(int const socket)
{
	if (DEBUG)
		std::cout << "setSocket() member function of client called\n";
	this->_socket = socket;
}

void	Client::setName(std::string const name)
{
	if (DEBUG)
		std::cout << "setName() member function of client called\n";
	this->_name = name;
}

void	Client::setNickname(std::string const nickname)
{
	if (DEBUG)
		std::cout << "setNickname() member function of client called\n";
	this->_nickname = nickname;
}

void	Client::setEvent()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";
	this->_client_event = new struct epoll_event;
	this->_client_event->events = EPOLLIN;
	this->_client_event->data.fd = this->_socket;
}

void	Client::setSockAddr(sockaddr_in const &addr)
{
	if (DEBUG)
		std::cout << "setSockAddr() member function of client called\n";
	this->_sock_addr = addr;
}

// Methods //


