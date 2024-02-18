/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:45:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 00:15:27 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Client instance, initializing its fields with given arguments.
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
	int const       socket,
	Nickname const &nickname,
	Hostname const &hostname,
	Username const &username,
	Realname const &realname,
	UserModes const modes) :
	_socket(socket),
	_messages(),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(modes)
{
}

/**
 * @brief Constructs a new Client instance, copying a given source Client instance.
 *
 * @param src The source Client instance to copy.
 *
 * @return The newly created Client instance.
 */
Client::Client(Client const &src) :
	_socket(src._socket),
	_messages(src._messages),
	_nickname(src._nickname),
	_hostname(src._hostname),
	_username(src._username),
	_realname(src._realname),
	_modes(src._modes)
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
std::string const &Client::get_messages(void) const { return this->_messages; }
Nickname const    &Client::get_nickname(void) const { return this->_nickname; }
Hostname const    &Client::get_hostname(void) const { return this->_hostname; }
Username const    &Client::get_username(void) const { return this->_username; }
Realname const    &Client::get_realname(void) const { return this->_realname; }
UserModes          Client::get_modes(void) const { return this->_modes; }

// ****************************************************************************************************************** //
//                                                      Mutators                                                      //
// ****************************************************************************************************************** //
void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_messages(std::string const &messages) { this->_messages = messages; }
void Client::set_nickname(Nickname const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(Hostname const &hostname) { this->_hostname = hostname; }
void Client::set_username(Username const &username) { this->_username = username; }
void Client::set_realname(Realname const &realname) { this->_realname = realname; }
void Client::set_modes(UserModes const modes) { this->_modes = modes; }

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
void Client::set_mode(UserMode const mode) { this->_modes.set(mode); }

/**
 * @brief Clears a given mode for the client.
 *
 * @param mode The mode to clear.
 */
void Client::clear_mode(UserMode const mode) { this->_modes.clear(mode); }

/**
 * @brief Check whether the client has a given mode set.
 *
 * @param mode The mode to check.
 *
 * @return true if the client has the given mode set, false otherwise.
 */
bool Client::has_mode(UserMode const mode) const { return this->_modes.is_set(mode); }

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
