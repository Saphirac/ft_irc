/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:20:40 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/20 21:21:47 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"

Client::Client(
	int const          socket,
	std::string const &nickname,
	std::string const &hostname,
	std::string const &username,
	std::string const &realname,
	uint8_t const      modes) :
	_socket(socket),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(modes),
	_time_last_msg(std::clock())
{
	if (DEBUG)
		std::cout << "Client constructor called\n";
}

Client::Client(Client const &src) :
	_socket(src._socket),
	_messages(src._messages),
	_nickname(src._nickname),
	_hostname(src._hostname),
	_username(src._username),
	_realname(src._realname),
	_modes(src._modes)
{
	if (DEBUG)
		std::cout << "Client copy constructor called\n";
}

Client::~Client()
{
	if (DEBUG)
		std::cout << "Client destructor called\n";
	if (this->_socket != -1)
		close(this->_socket);
	// if (this->_epoll_event)
	//	delete this->_epoll_event;
}

// Getters //

int                 Client::get_socket(void) const { return this->_socket; }
std::string const  &Client::get_messages(void) const { return this->_messages; }
std::string const  &Client::get_nickname(void) const { return this->_nickname; }
std::string const  &Client::get_hostname(void) const { return this->_hostname; }
std::string const  &Client::get_username(void) const { return this->_username; }
std::string const  &Client::get_realname(void) const { return this->_realname; }
uint8_t             Client::get_modes(void) const { return this->_modes; }
struct epoll_event *Client::get_epoll_event(void) const { return this->_epoll_event; }
bool                Client::get_is_msg_complete(void) const { return this->_is_msg_complete; }
std::clock_t        Client::get_time_last_msg(void) const { return this->_time_last_msg; }

// Setters //

void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_messages(std::string const &messages) { this->_messages = messages; }
void Client::set_nickname(std::string const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(std::string const &hostname) { this->_hostname = hostname; }
void Client::set_username(std::string const &username) { this->_username = username; }
void Client::set_realname(std::string const &realname) { this->_realname = realname; }
void Client::set_modes(uint8_t const modes) { this->_modes = modes; }

void Client::set_is_msg_complete(bool is_msg_complete) { this->_is_msg_complete = is_msg_complete; }

void Client::set_epoll_event()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";
	this->_epoll_event = new epoll_event;
	printf("epoll_event address : %p\n", this->_epoll_event);
	this->_epoll_event->events = EPOLLIN;
	this->_epoll_event->data.fd = this->_socket;
}

void Client::set_time_last_msg(void) { this->_time_last_msg = std::clock(); }

// Methods //
/**
 * @brief Closes the socket of the Client instance.
 */
void Client::disconnect(void)
{
	close(this->_socket);
	this->_socket = -1;
}
