/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/17 18:48:05 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"

Client::Client(
	int const           socket,
	std::string const  &nickname,
	std::string const  &hostname,
	std::string const  &username,
	std::string const  &realname,
	sockaddr_in *sock_addr,
	uint8_t const       modes) :
	_socket(socket),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(modes),
	_sock_addr(sock_addr),
	_addr_len(sizeof(sock_addr))
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
	_modes(src._modes),
	_sock_addr(src._sock_addr),
	_addr_len(src._addr_len)
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
	//if (this->_epoll_event)
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
struct sockaddr_in *Client::get_sock_addr(void) const { return this->_sock_addr; }
socklen_t           Client::get_sock_len(void) const { return this->_addr_len; }
struct epoll_event *Client::get_epoll_event(void) const { return this->_epoll_event; }
bool                Client::get_is_complete(void) const { return this->_is_complete; }
bool                Client::get_is_msg_complete(void) const { return this->_is_msg_complete; }
bool                Client::get_is_pass(void) const { return this->_pass; }

// Setters //

void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_messages(std::string const &messages) { this->_messages = messages; }
void Client::set_nickname(std::string const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(std::string const &hostname) { this->_hostname = hostname; }
void Client::set_username(std::string const &username) { this->_username = username; }
void Client::set_realname(std::string const &realname) { this->_realname = realname; }
void Client::set_modes(uint8_t const modes) { this->_modes = modes; }

void Client::set_is_complete(bool is_complete) { this->_is_complete = is_complete; }
void Client::set_is_msg_complete(bool is_msg_complete) { this->_is_msg_complete = is_msg_complete; }
void Client::set_is_pass(bool is_pass) { this->_pass = is_pass; }

void Client::set_sock_addr(struct sockaddr_in *sock_addr) { this->_sock_addr = sock_addr; }
void Client::set_sock_len() { this->_addr_len = sizeof(this->_sock_addr); }

void Client::set_epoll_event()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";
	this->_epoll_event = new struct epoll_event;
	this->_epoll_event->events = EPOLLIN;
	this->_epoll_event->data.fd = this->_socket;
}

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
 * @brief Appends a given message to the messages of the Client instance.
 *
 * @param message The message to append.
 */
void Client::append_message(std::string const &message) { this->_messages += message + "\r\n"; }

/**
 * @brief Clears the messages of the Client instance.
 */
void Client::clear_messages(void) { this->_messages.clear(); }

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
 * @brief Sends a given message on the socket of the Client instance.
 *
 * @param message The message to send.
 *
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t Client::send_message(std::string const &message) const
{
	return send(this->_socket, message.c_str(), message.size(), 0);
}

/**
 * @brief Sends the messages that are currently stored in the Client instance on its socket.
 *
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t Client::send_messages(void) const
{
	return send(this->_socket, this->_messages.c_str(), this->_messages.size(), 0);
}

/**
 * @brief Generates the user mask of the client.
 *
 * @return The user mask of the client.
 */
std::string Client::user_mask(void) const { return this->_nickname + "!" + this->_username + "@" + this->_hostname; }
