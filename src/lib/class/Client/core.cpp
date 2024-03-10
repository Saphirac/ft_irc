/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:46:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 00:25:03 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/exception/ProblemWithClock.hpp"

// Shared fields //

std::string const Client::_default_quit_msg = "Leaving, bye! <3";

// Constructors //

Client::Modes::Flags::Flags(void) : _bits(0) {}

Client::Modes::Modes(void) : _flags(), _away_msg() {}

/**
 * @param socket The socket of the client.
 * @param nickname The nickname of the client.
 * @param hostname The hostname of the client.
 * @param username The username of the client.
 * @param realname The realname of the client.
 *
 * @throw `ProblemWithClock` if `clock()` fails.
 */
Client::Client(
	int const       socket,
	NickName const &nickname,
	HostName const &hostname,
	UserName const &username,
	RealName const &realname) :
	_socket(socket),
	_msg_in(),
	_msg_out(),
	_last_msg_time(std::clock()),
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_has_been_pinged(false),
	_modes(),
	_joined_channels_by_name()
{
	if (this->_last_msg_time == -1)
		throw ProblemWithClock();
}

// Destructor //

Client::Modes::Flags::~Flags(void) {}

Client::Modes::~Modes(void) {}

/**
 * @throw `ProblemWithClose` if `close()` fails.
 */
Client::~Client(void) { this->disconnect(); }

// Accessors //

std::string const   &Client::Modes::get_away_msg(void) const { return this->_away_msg; }
int                  Client::get_socket(void) const { return this->_socket; }
NickName const      &Client::get_nickname(void) const { return this->_nickname; }
HostName const      &Client::get_hostname(void) const { return this->_hostname; }
Client::Modes const &Client::get_modes(void) const { return this->_modes; }
bool                 Client::get_has_been_pinged(void) const { return this->_has_been_pinged; }

// Mutators //

void Client::set_socket(int const socket) { this->_socket = socket; }
void Client::set_last_msg_time(std::clock_t const time) { this->_last_msg_time = time; }
void Client::set_nickname(NickName const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(HostName const &hostname) { this->_hostname = hostname; }
void Client::set_username(UserName const &username) { this->_username = username; }
void Client::set_realname(RealName const &realname) { this->_realname = realname; }
