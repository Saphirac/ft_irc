/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:45:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/04 05:05:09 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "UserMode.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Client instance, initializing its fields with the given parameters.
 *
 * @param socket The socket of the client.
 * @param nickname The nickname of the client.
 * @param hostname The hostname of the client.
 * @param username The username of the client.
 * @param realname The realname of the client.
 * @param modes The modes of the client.
 *
 * @return The newly created Client instance.
 */
Client::Client(
	int const          socket,
	std::string const &nickname,
	std::string const &hostname,
	std::string const &username,
	std::string const &realname,
	uint8_t            modes) :
	_socket(socket),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(modes)
{
}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Client::~Client(void)
{
	if (this->_socket != -1)
		close(this->_socket);
}

// ***************************************************************************************************************** //
//                                                     Accessors                                                     //
// ***************************************************************************************************************** //
int                Client::get_socket(void) const { return this->_socket; }
std::string const &Client::get_nickname(void) const { return this->_nickname; }
std::string const &Client::get_hostname(void) const { return this->_hostname; }
std::string const &Client::get_username(void) const { return this->_username; }
std::string const &Client::get_realname(void) const { return this->_realname; }
uint8_t            Client::get_modes(void) const { return this->_modes; }

// ****************************************************************************************************************** //
//                                                      Mutators                                                      //
// ****************************************************************************************************************** //
void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_nickname(std::string const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(std::string const &hostname) { this->_hostname = hostname; }
void Client::set_username(std::string const &username) { this->_username = username; }
void Client::set_realname(std::string const &realname) { this->_realname = realname; }
void Client::set_modes(uint8_t const modes) { this->_modes = modes; }

// ***************************************************************************************************************** //
//                                              Public Member Functions                                              //
// ***************************************************************************************************************** //
/**
 * @brief Closes the socket of the Client instance.
 */
void Client::disconnect(void)
{
	close(this->_socket);
	this->_socket = -1;
}

/**
 * @brief Checks whether the client has already sent the PASS command.
 *
 * @return true if the client has already sent the PASS command, false otherwise.
 */
bool Client::has_already_sent_pass(void) const { return this->_modes & 1 << UserModePass; }

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