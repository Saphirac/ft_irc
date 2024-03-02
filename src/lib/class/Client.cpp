/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:45:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/01 23:04:13 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/Exceptions.hpp"
#include <sys/socket.h>
#include <unistd.h>

// ***************************************************************************************************************** //
//                                                   Shared Fields                                                   //
// ***************************************************************************************************************** //
std::string const Client::_default_quit_msg = "Leaving, bye! <3";

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
Client::Modes::Flags::Flags(void) : _bits(0) {}

Client::Modes::Modes(void) : _flags(), _away_msg() {}

/**
 * @param socket The socket of the client.
 * @param nickname The nickname of the client.
 * @param hostname The hostname of the client.
 * @param username The username of the client.
 * @param realname The realname of the client.
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
	_nickname(nickname),
	_hostname(hostname),
	_username(username),
	_realname(realname),
	_modes(),
	_joined_channels_by_name()
{
}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Client::Modes::Flags::~Flags(void) {}

Client::Modes::~Modes(void) {}

Client::~Client(void) { this->disconnect(); }

// ***************************************************************************************************************** //
//                                                     Accessors                                                     //
// ***************************************************************************************************************** //
int                  Client::get_socket(void) const { return this->_socket; }
NickName const      &Client::get_nickname(void) const { return this->_nickname; }
HostName const      &Client::get_hostname(void) const { return this->_hostname; }
Client::Modes const &Client::get_modes(void) const { return this->_modes; }

// ****************************************************************************************************************** //
//                                                      Mutators                                                      //
// ****************************************************************************************************************** //
std::string const &Client::Modes::get_away_msg(void) const { return this->_away_msg; }

void Client::set_nickname(NickName const &nickname) { this->_nickname = nickname; }
void Client::set_hostname(HostName const &hostname) { this->_hostname = hostname; }
void Client::set_username(UserName const &username) { this->_username = username; }
void Client::set_realname(RealName const &realname) { this->_realname = realname; }

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Sets a flag.
 *
 * @param flag The flag to set.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
void Client::Modes::Flags::set(UserMode const flag)
{
	switch (flag)
	{
	case Bot:
		this->_bits |= 1 << Bot;
		return;
	case LocalOperator:
		this->_bits |= 1 << LocalOperator;
		return;
	case WallopsListener:
		this->_bits |= 1 << WallopsListener;
		return;
	case Invisible:
		this->_bits |= 1 << Invisible;
		return;
	case AlreadySentPass:
		this->_bits |= 1 << AlreadySentPass;
		return;
	case AlreadySentUser:
		this->_bits |= 1 << AlreadySentUser;
		return;
	default:
		throw NotAFlag();
	}
}

/**
 * @brief Clears a flag.
 *
 * @param flag The flag to clear.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
void Client::Modes::Flags::clear(UserMode const flag)
{
	switch (flag)
	{
	case Bot:
		this->_bits &= ~(1 << Bot);
		return;
	case LocalOperator:
		this->_bits &= ~(1 << LocalOperator);
		return;
	case WallopsListener:
		this->_bits &= ~(1 << WallopsListener);
		return;
	case Invisible:
		this->_bits &= ~(1 << Invisible);
		return;
	case AlreadySentPass:
		this->_bits &= ~(1 << AlreadySentPass);
		return;
	case AlreadySentUser:
		this->_bits &= ~(1 << AlreadySentUser);
		return;
	default:
		throw NotAFlag();
	}
}

/**
 * @brief Checks whether a flag is set.
 *
 * @param flag The flag to check.
 *
 * @return `true` if the flag is set, `false` otherwise.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
bool Client::Modes::Flags::is_set(UserMode const flag) const
{
	switch (flag)
	{
	case Bot:
		return this->_bits & 1 << Bot;
	case LocalOperator:
		return this->_bits & 1 << LocalOperator;
	case WallopsListener:
		return this->_bits & 1 << WallopsListener;
	case Invisible:
		return this->_bits & 1 << Invisible;
	case AlreadySentPass:
		return this->_bits & 1 << AlreadySentPass;
	case AlreadySentUser:
		return this->_bits & 1 << AlreadySentUser;
	default:
		throw NotAFlag();
	}
}

/**
 * @return The string representation of the flags that are currently set.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Client::Modes::Flags::to_string(void) const
{
	std::string flags_as_string;

	if (this->is_set(Bot))
		flags_as_string += USER_MODES[Bot];
	if (this->is_set(LocalOperator))
		flags_as_string += USER_MODES[LocalOperator];
	if (this->is_set(WallopsListener))
		flags_as_string += USER_MODES[WallopsListener];
	if (this->is_set(Invisible))
		flags_as_string += USER_MODES[Invisible];

	return flags_as_string;
}

/**
 * @brief Sets a mode.
 *
 * @param mode The mode to set.
 */
void Client::Modes::set(UserMode const mode, void const *const arg)
{
	switch (mode)
	{
	case Bot:
		this->_flags.set(Bot);
		return;
	case LocalOperator:
		this->_flags.set(LocalOperator);
		return;
	case WallopsListener:
		this->_flags.set(WallopsListener);
		return;
	case Invisible:
		this->_flags.set(Invisible);
		return;
	case Away:
		this->_away_msg = *static_cast<std::string const *>(arg);
		return;
	case AlreadySentPass:
		this->_flags.set(AlreadySentPass);
		return;
	case AlreadySentUser:
		this->_flags.set(AlreadySentUser);
		return;
	}
}

/**
 * @brief Clears a mode.
 *
 * @param mode The mode to clear.
 */
void Client::Modes::clear(UserMode const mode)
{
	switch (mode)
	{
	case Bot:
		return this->_flags.clear(Bot);
	case LocalOperator:
		return this->_flags.clear(LocalOperator);
	case WallopsListener:
		return this->_flags.clear(WallopsListener);
	case Invisible:
		return this->_flags.clear(Invisible);
	case Away:
		return this->_away_msg.clear();
	case AlreadySentPass:
		return this->_flags.clear(AlreadySentPass);
	case AlreadySentUser:
		return this->_flags.clear(AlreadySentUser);
	}
}

/**
 * @brief Checks whether a mode is set.
 *
 * @param mode The mode to check.
 *
 * @return `true` if the mode is set, `false` otherwise.
 */
bool Client::Modes::is_set(UserMode const mode) const
{
	switch (mode)
	{
	case Bot:
		return this->_flags.is_set(Bot);
	case LocalOperator:
		return this->_flags.is_set(LocalOperator);
	case WallopsListener:
		return this->_flags.is_set(WallopsListener);
	case Invisible:
		return this->_flags.is_set(Invisible);
	case Away:
		return !this->_away_msg.empty();
	case AlreadySentPass:
		return this->_flags.is_set(AlreadySentPass);
	case AlreadySentUser:
		return this->_flags.is_set(AlreadySentUser);
	}
}

/**
 * @return The string representation of the modes that are currently set.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Client::Modes::to_string(void) const { return this->_flags.to_string(); }

/**
 * @brief Closes the socket of the Client instance.
 */
void Client::disconnect(void)
{
	if (this->_socket != -1)
	{
		close(this->_socket);
		this->_socket = -1;
	}
}

/**
 * @brief Appends a string to the input buffer of the Client instance.
 *
 * @param s The string to append.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Client::append_to_msg_in(std::string const &s) { this->_msg_in += s; }

/**
 * @brief Appends a message to the output buffer of the Client instance. The message is suffixed with a CRLF
 * sequence.
 *
 * @param msg The message to append.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Client::append_to_msg_out(std::string const &msg) { this->_msg_out += msg + "\r\n"; }

/**
 * @brief Clears the output buffer of the Client instance.
 */
void Client::clear_msg_out(void) { this->_msg_out.clear(); }

/**
 * @brief Sets a mode for the client.
 *
 * @param mode The mode to set.
 */
void Client::set_mode(UserMode const mode, void const *const arg) { this->_modes.set(mode, arg); }

/**
 * @brief Clears a mode for the client.
 *
 * @param mode The mode to clear.
 */
void Client::clear_mode(UserMode const mode) { this->_modes.clear(mode); }

/**
 * @brief Check whether a mode is set for the client.
 *
 * @param mode The mode to check.
 *
 * @return `true` if the mode is set for the client, `false` otherwise.
 */
bool Client::has_mode(UserMode const mode) const { return this->_modes.is_set(mode); }

/**
 * @brief Generates the user mask of the client.
 *
 * @return The user mask of the client.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Client::user_mask(void) const { return this->_nickname + "!" + this->_username + "@" + this->_hostname; }

/**
 * @brief
 * Sends the content of the output buffer of the Client instance to its socket.
 * Upon success, the output buffer is cleared.
 *
 * @throw `ProblemWithSend` if the message cannot be sent.
 */
void Client::send_msg_out(void)
{
	if (send(this->_socket, this->_msg_out.c_str(), this->_msg_out.size(), 0) == -1)
		throw ProblemWithSend();

	this->clear_msg_out();
}
