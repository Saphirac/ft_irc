/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/19 14:52:22 by mcourtoi         ###   ########.fr       */
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
	_time_since_last_msg(std::clock() / CLOCKS_PER_SEC),
	_time_since_last_ping(-1)
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
	_time_since_last_msg(src._time_since_last_msg),
	_time_since_last_ping(src._time_since_last_ping)
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
std::string const  &Client::get_msg_in(void) const { return this->_msg_in; }
std::string const  &Client::get_msg_out(void) const { return this->_msg_out; }
std::string const  &Client::get_nickname(void) const { return this->_nickname; }
std::string const  &Client::get_hostname(void) const { return this->_hostname; }
std::string const  &Client::get_username(void) const { return this->_username; }
std::string const  &Client::get_realname(void) const { return this->_realname; }
uint8_t             Client::get_modes(void) const { return this->_modes; }
struct epoll_event *Client::get_epoll_event(void) const { return this->_epoll_event; }
bool                Client::get_is_complete(void) const { return this->_is_complete; }
bool                Client::get_is_pass(void) const { return this->_pass; }
std::clock_t        Client::get_time_last_msg(void) const { return this->_time_since_last_msg; }
std::clock_t        Client::get_time_last_ping(void) const { return this->_time_since_last_ping; }

// Setters //

void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_msg_in(std::string const &msg) { this->_msg_in = msg; }
void Client::set_msg_out(std::string const &msg) { this->_msg_out = msg; }
void Client::set_nickname(std::string const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(std::string const &hostname) { this->_hostname = hostname; }
void Client::set_username(std::string const &username) { this->_username = username; }
void Client::set_realname(std::string const &realname) { this->_realname = realname; }
void Client::set_modes(uint8_t const modes) { this->_modes = modes; }

void Client::set_is_complete(bool is_complete) { this->_is_complete = is_complete; }
void Client::set_is_pass(bool is_pass) { this->_pass = is_pass; }

void Client::set_epoll_event()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";
	this->_epoll_event = new epoll_event;
	printf("epoll_event address : %p\n", this->_epoll_event);
	this->_epoll_event->events = EPOLLIN;
	this->_epoll_event->data.fd = this->_socket;
}

void Client::set_time_last_msg(void) { this->_time_since_last_msg = std::clock() / CLOCKS_PER_SEC; }
void Client::set_time_last_ping(void) { this->_time_since_last_ping = std::clock() / CLOCKS_PER_SEC; }

// Methods //
/**
 * @brief Closes the socket of the Client instance.
 */
void Client::disconnect(void)
{
	close(this->_socket);
	this->_socket = -1;
}

/**
 * @brief Sets a given mode for the client.
 *
 * @param mode The mode to set.
 */
void Client::set_mode(UserMode const mode) { this->_modes |= 1 << mode; }

/**
 * @brief Clears a given mode for the client.
 *
 * @param mode The mode to clear.
 */
void Client::clear_mode(UserMode const mode) { this->_modes &= ~(1 << mode); }

/**
 * @brief Check whether the client has a given mode set.
 *
 * @param mode The mode to check.
 *
 * @return true if the client has the given mode set, false otherwise.
 */
bool Client::has_mode(UserMode const mode) const { return this->_modes & 1 << mode; }

/**
 * @brief Generates the user mask of the client.
 *
 * @return The user mask of the client.
 */
std::string Client::user_mask(void) const { return this->_nickname + "!" + this->_username + "@" + this->_hostname; }

std::clock_t Client::check_time_since_last_msg(void) const
{
	return ((std::clock() - this->_time_since_last_msg) / CLOCKS_PER_SEC);
}
std::clock_t Client::check_time_since_last_ping(void) const
{
	return ((std::clock() - this->_time_since_last_ping) / CLOCKS_PER_SEC);
}
