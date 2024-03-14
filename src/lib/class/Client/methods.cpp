/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 23:45:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 00:10:49 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/exception/InvalidConversion.hpp"
#include "class/exception/NotAFlag.hpp"
#include "class/exception/ProblemWithClock.hpp"
#include "class/exception/ProblemWithClose.hpp"
#include "class/exception/ProblemWithSend.hpp"
#include "class/exception/UnknownReply.hpp"
#include "replies.hpp"
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
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
	case IsAboutToBeDisconnected:
		this->_bits |= 1 << IsAboutToBeDisconnected;
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
	case IsAboutToBeDisconnected:
		this->_bits &= ~(1 << IsAboutToBeDisconnected);
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
		return (this->_bits & 1 << Bot) != 0;
	case LocalOperator:
		return (this->_bits & 1 << LocalOperator) != 0;
	case WallopsListener:
		return (this->_bits & 1 << WallopsListener) != 0;
	case Invisible:
		return (this->_bits & 1 << Invisible) != 0;
	case AlreadySentPass:
		return (this->_bits & 1 << AlreadySentPass) != 0;
	case AlreadySentUser:
		return (this->_bits & 1 << AlreadySentUser) != 0;
	case IsAboutToBeDisconnected:
		return (this->_bits & 1 << IsAboutToBeDisconnected) != 0;
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
 * @brief Sets a mode for the client.
 *
 * @param mode The mode to set.
 * @param arg The argument associated with the mode if any.
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
	case IsAboutToBeDisconnected:
		this->_flags.set(IsAboutToBeDisconnected);
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
	case IsAboutToBeDisconnected:
		return this->_flags.clear(IsAboutToBeDisconnected);
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
	case IsAboutToBeDisconnected:
		return this->_flags.is_set(IsAboutToBeDisconnected);
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

#define TERMINATING_SEQUENCE        "\r\n"
#define TERMINATING_SEQUENCE_LENGTH sizeof(TERMINATING_SEQUENCE) - 1
#define MAXIMUM_LENGTH_FOR_MESSAGE  512
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

	this->_msg_in.erase(0, pos + TERMINATING_SEQUENCE_LENGTH);
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
clock_t Client::time_since_last_msg(void) const
{
	clock_t const now = clock();

	if (now == -1)
		throw ProblemWithClock();

	return now - this->_last_msg_time;
}

/**
 * @brief Generates the prefix of the client, with 1 leading colon and 1 trailing space.
 *
 * @return The prefix of the client.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Client::prefix(void) const
{
	return ':' + this->_nickname + '!' + this->_username + '@' + this->_hostname + ' ';
}

/**
 * @brief Computes the `%c` conversion specification.
 *
 * @param format The format string to insert the argument in.
 * @param length_modifiers The length modifiers of the conversion specification.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return
 * The number of bytes inserted in the format string, or `std::string::npos` if the conversion specification is invalid.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static size_t convert_c(
	std::string       &format,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (!length_modifiers.empty())
	{
		if (length_modifiers != "l")
			return std::string::npos;

		wchar_t const arg = static_cast<wchar_t>(va_arg(args, int));
		int const     byte_count = mblen(reinterpret_cast<char const *>(&arg), sizeof(wchar_t));

		if (byte_count == -1)
		{
			format.erase(percent_pos, specifier_pos - percent_pos + 1);
			return 0;
		}

		format.replace(
			percent_pos,
			specifier_pos - percent_pos + 1,
			reinterpret_cast<char const *>(&arg),
			static_cast<size_t>(byte_count));
		return static_cast<size_t>(byte_count);
	}

	char const arg = static_cast<char>(va_arg(args, int));

	format.replace(percent_pos, specifier_pos - percent_pos + 1, 1, arg);
	return 1;
}

/**
 * @brief Computes the `%s` conversion specification.
 *
 * @param format The format string to insert the argument in.
 * @param length_modifiers The length modifiers of the conversion specification.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return
 * The number of bytes inserted in the format string, or `std::string::npos` if the conversion specification is invalid.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static size_t convert_s(
	std::string       &format,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (!length_modifiers.empty() && length_modifiers != "l")
		return std::string::npos;

	char const *const arg = va_arg(args, char const *);

	format.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return strlen(arg);
}

/**
 * @brief Computes the `%S` conversion specification.
 *
 * @param format The format string to insert the argument in.
 * @param length_modifiers The length modifiers of the conversion specification.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return
 * The number of bytes inserted in the format string, or `std::string::npos` if the conversion specification is invalid.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static size_t convert_S(
	std::string       &format,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (!length_modifiers.empty())
	{
		if (length_modifiers != "l")
			return std::string::npos;

		std::wstring const arg = *va_arg(args, std::wstring const *);

		format.replace(percent_pos, specifier_pos - percent_pos + 1, reinterpret_cast<char const *>(arg.c_str()));
		return arg.size() * sizeof(wchar_t);
	}

	std::string const arg = *va_arg(args, std::string const *);

	format.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes either the `%hhu` or the `%hu` conversion specification.
 *
 * @tparam Uint The type of the unsigned integer to convert. (assumed to be either `uint8_t` or `uint16_t`)
 *
 * @param format The format string to insert the argument in.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return The number of bytes inserted in the format string.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
template<typename Uint>
inline static size_t convert_low_uint(
	std::string &format,
	size_t const percent_pos,
	size_t const specifier_pos,
	va_list      args)
{
	std::stringstream ss;

	ss << static_cast<Uint>(va_arg(args, uint32_t));

	std::string const arg = ss.str();

	format.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes either the `%lu` or the `%llu` conversion specification.
 *
 * @tparam Uint The type of the unsigned integer to convert. (assumed to be either `uint32_t` or `uint64_t`)
 *
 * @param format The format string to insert the argument in.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return The number of bytes inserted in the format string.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
template<typename Uint>
inline static size_t convert_high_uint(
	std::string &format,
	size_t const percent_pos,
	size_t const specifier_pos,
	va_list      args)
{
	std::stringstream ss;

	ss << va_arg(args, Uint);

	std::string const arg = ss.str();

	format.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes the `%u` conversion specification.
 *
 * @param format The format string to insert the argument in.
 * @param length_modifiers The length modifiers of the conversion specification.
 * @param percent_pos The position of the '%' character in the format string.
 * @param specifier_pos The position of the specifier character in the format string.
 * @param args The list of variadic arguments.
 *
 * @return
 * The number of bytes inserted in the format string, or `std::string::npos` if the conversion specification is invalid.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
inline static size_t convert_u(
	std::string       &format,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (length_modifiers == "hh")
		return convert_low_uint<uint8_t>(format, percent_pos, specifier_pos, args);
	if (length_modifiers == "h")
		return convert_low_uint<uint16_t>(format, percent_pos, specifier_pos, args);
	if (length_modifiers.empty())
		return convert_high_uint<uint32_t>(format, percent_pos, specifier_pos, args);
	if (length_modifiers == "l" || length_modifiers == "ll")
		return convert_high_uint<uint64_t>(format, percent_pos, specifier_pos, args);
	return std::string::npos;
}

/**
 * @brief
 * Generates a formatted reply message corresponding to a given reply number.
 * Puts the prefix of the client and their nickname as the target of the message.
 * Uses the given variadic arguments to replace the conversion specifications in the raw reply message.
 *
 * @param reply_number The number of the reply to format.
 * @param ... The variadic arguments to use for the conversion specifications.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Client::append_formatted_reply_to_msg_out(int const reply_number...)
{
	ReplyIterator const format_by_reply = formats_by_reply.find(reply_number);

	if (format_by_reply == formats_by_reply.end())
		throw UnknownReply();

	std::string format = format_by_reply->second;
	size_t      percent_pos = format.find('%');
	va_list     args;

	va_start(args, reply_number);
	while (percent_pos != std::string::npos)
	{
		size_t const      specifier_pos = format.find_first_not_of("hl", percent_pos + 1);
		std::string const length_modifiers = format.substr(percent_pos + 1, specifier_pos - percent_pos - 1);
		size_t            inserted_len;

		switch (format[specifier_pos])
		{
		case 'c':
			inserted_len = convert_c(format, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 's':
			inserted_len = convert_s(format, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 'S':
			inserted_len = convert_S(format, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 'u':
			inserted_len = convert_u(format, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case '%':
			if (!length_modifiers.empty())
			{
				va_end(args);
				throw InvalidConversion();
			}

			format.erase(percent_pos, 1);
			inserted_len = 1;
			break;
		default:
			va_end(args);
			throw InvalidConversion();
		}
		if (inserted_len == std::string::npos)
		{
			va_end(args);
			throw InvalidConversion();
		}
		percent_pos = format.find('%', percent_pos + inserted_len);
	}
	va_end(args);

	std::stringstream ss;

	ss << this->prefix() << std::setw(3) << std::setfill('0') << reply_number << ' ' << this->_nickname << ' '
	   << format;

	this->append_to_msg_out(ss.str());
}
// TODO: implement unit tests for this function

/**
 * @brief Appends a message to the output buffer of the Client instance. The message is suffixed with a CRLF
 * sequence.
 *
 * @param msg The message to append.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Client::append_to_msg_out(std::string const &msg) { this->_msg_out += msg + TERMINATING_SEQUENCE; }

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

	this->_msg_out.clear();
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
	if (this->_socket != -1)
	{
		if (close(this->_socket))
			throw ProblemWithClose();
		this->_socket = -1;
	}
}
