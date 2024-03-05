/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:45:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:32:44 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/exception/NotAFlag.hpp"
#include "class/exception/ProblemWithClock.hpp"
#include "class/exception/ProblemWithClose.hpp"
#include "class/exception/ProblemWithSend.hpp"
#include <sys/socket.h>
#include <unistd.h>

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
 * @brief Appends a string to the input buffer of the Client instance.
 *
 * @param s The string to append.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Client::append_to_msg_in(std::string const &s) { this->_msg_in += s; }

#define TERMINATING_SEQUENCE       "\r\n"
#define MAXIMUM_LENGTH_FOR_MESSAGE 512
/**
 * @brief Get the first message in the input buffer of the Client instance.
 * A message is <= 512 characters and is suffixed with a CRLF sequence.
 * Then it extracts and return this message and erase it from the reste of the buffer.
 *
 * @return The message found or an empty string if either no message is found or the message is too long.
 */
std::string Client::get_next_msg(void)
{
	size_t const pos = this->_msg_in.find(TERMINATING_SEQUENCE);

	if (pos == std::string::npos)
		return std::string();

	std::string const msg = this->_msg_in.substr(0, pos);

	this->_msg_in.erase(0, pos + sizeof(TERMINATING_SEQUENCE));
	if (pos > MAXIMUM_LENGTH_FOR_MESSAGE)
		return std::string();

	return msg;
}

/**
 * @brief Compares the time since the last message was sent by the client to the current time.
 *
 * @return The difference between the current time and the time of the last message.
 *
 * @throw `ProblemWithClock` if `clock()` fails.
 */
std::clock_t Client::time_since_last_msg(void) const
{
	std::clock_t const now = std::clock();

	if (now == -1)
		throw ProblemWithClock();

	return now - this->_last_msg_time;
}

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
 * @brief
 * Sends the content of the output buffer of the Client instance to its socket.
 * Upon success, the output buffer is cleared.
 *
 * @throw `ProblemWithSend` if `send()` fails.
 */
void Client::send_msg_out(void)
{
	if (this->_msg_out.empty())
		return;

	if (send(this->_socket, this->_msg_out.c_str(), this->_msg_out.size(), 0) == -1)
		throw ProblemWithSend();

	this->clear_msg_out();
}

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
 * @brief Closes the socket of the Client instance.
 *
 * @throw `ProblemWithClose` if `close()` fails.
 */
void Client::disconnect(void)
{
	if (close(this->_socket))
		throw ProblemWithClose();
	this->_socket = -1;
}
