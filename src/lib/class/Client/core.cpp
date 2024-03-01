/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:20:40 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/01 22:13:44 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client(
	int const       socket,
	Nickname const &nickname,
	Hostname const &hostname,
	Username const &username,
	Realname const &realname,
	UserModes const modes) :
	_socket(socket),
	_msg_in(),
	_msg_out(),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(modes),
	_epoll_event(this->set_epoll_event()),
	_time_last_msg(std::clock())
{
	if (DEBUG)
		std::cout << "Client constructor called\n";
}

Client::Client(Client const &src) :
	_socket(src._socket),
	_msg_in(src._msg_in),
	_msg_out(src._msg_out),
	_nickname(src._nickname),
	_hostname(src._hostname),
	_username(src._username),
	_realname(src._realname),
	_modes(src._modes),
	_time_last_msg(src._time_last_msg)
{
	if (DEBUG)
		std::cout << "Client copy constructor called\n";
}

Client::~Client()
{
	if (DEBUG)
		std::cout << "Client destructor called\n";
	this->disconnect();
}

// Getters //

int                Client::get_socket(void) const { return this->_socket; }
std::string const &Client::get_msg_in(void) const { return this->_msg_in; }
std::string const &Client::get_msg_out(void) const { return this->_msg_out; }
Nickname const    &Client::get_nickname(void) const { return this->_nickname; }
Hostname const    &Client::get_hostname(void) const { return this->_hostname; }
Username const    &Client::get_username(void) const { return this->_username; }
Realname const    &Client::get_realname(void) const { return this->_realname; }
UserModes          Client::get_modes(void) const { return this->_modes; }
std::clock_t       Client::get_time_last_msg(void) const { return this->_time_last_msg; }
epoll_event       *Client::get_mut_epoll_event(void) const { return this->_epoll_event; }

// Setters //

void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_msg_in(std::string const &s) { this->_msg_in = s; }
void Client::set_msg_out(std::string const &msg) { this->_msg_out = msg; }
void Client::set_nickname(Nickname const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(Hostname const &hostname) { this->_hostname = hostname; }
void Client::set_username(Username const &username) { this->_username = username; }
void Client::set_realname(Realname const &realname) { this->_realname = realname; }
void Client::set_modes(UserModes const modes) { this->_modes = modes; }

/**
 * @brief Create a new struct epoll event specially for this client and assigns all necessary fields.
 *
 * @return epoll_event* The newly created epoll event.
 */
epoll_event *Client::set_epoll_event()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";

	epoll_event *e_event = new epoll_event;

	e_event->events = EPOLLIN;
	e_event->data.fd = this->_socket;
	return e_event;
}

void Client::set_time_last_msg(void) { this->_time_last_msg = std::clock(); }

// Methods //
/**
 * @brief Closes the socket of the Client instance.
 */
void Client::disconnect(void)
{
	std::cout << "Client disconnected.\n";
	close(this->_socket);
	this->_socket = -1;
	delete this->_epoll_event;
}
