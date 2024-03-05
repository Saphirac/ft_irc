/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_reply.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:49:58 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:10:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/InvalidConversion.hpp"
#include "class/exception/UnknownReply.hpp"
#include "replies.hpp"
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>

/**
 * @brief Computes the `%c` conversion specification.
 *
 * @param fmt The format string to insert the argument in.
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
	std::string       &fmt,
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
			fmt.erase(percent_pos, specifier_pos - percent_pos + 1);
			return 0;
		}

		fmt.replace(
			percent_pos,
			specifier_pos - percent_pos + 1,
			reinterpret_cast<char const *>(&arg),
			static_cast<size_t>(byte_count));
		return static_cast<size_t>(byte_count);
	}

	char const arg = static_cast<char>(va_arg(args, int));

	fmt.replace(percent_pos, specifier_pos - percent_pos + 1, 1, arg);
	return 1;
}

/**
 * @brief Computes the `%s` conversion specification.
 *
 * @param fmt The format string to insert the argument in.
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
	std::string       &fmt,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (!length_modifiers.empty() && length_modifiers != "l")
		return std::string::npos;

	char const *const arg = va_arg(args, char const *);

	fmt.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return strlen(arg);
}

/**
 * @brief Computes the `%S` conversion specification.
 *
 * @param fmt The format string to insert the argument in.
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
	std::string       &fmt,
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

		fmt.replace(percent_pos, specifier_pos - percent_pos + 1, reinterpret_cast<char const *>(arg.c_str()));
		return arg.size() * sizeof(wchar_t);
	}

	std::string const arg = *va_arg(args, std::string const *);

	fmt.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes either the `%hhu` or the `%hu` conversion specification.
 *
 * @tparam Uint The type of the unsigned integer to convert. (assumed to be either `uint8_t` or `uint16_t`)
 *
 * @param fmt The format string to insert the argument in.
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
	std::string &fmt,
	size_t const percent_pos,
	size_t const specifier_pos,
	va_list      args)
{
	std::stringstream ss;

	ss << static_cast<Uint>(va_arg(args, uint32_t));

	std::string const arg = ss.str();

	fmt.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes either the `%lu` or the `%llu` conversion specification.
 *
 * @tparam Uint The type of the unsigned integer to convert. (assumed to be either `uint32_t` or `uint64_t`)
 *
 * @param fmt The format string to insert the argument in.
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
	std::string &fmt,
	size_t const percent_pos,
	size_t const specifier_pos,
	va_list      args)
{
	std::stringstream ss;

	ss << va_arg(args, Uint);

	std::string const arg = ss.str();

	fmt.replace(percent_pos, specifier_pos - percent_pos + 1, arg);
	return arg.size();
}

/**
 * @brief Computes the `%u` conversion specification.
 *
 * @param fmt The format string to insert the argument in.
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
	std::string       &fmt,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (length_modifiers == "hh")
		return convert_low_uint<uint8_t>(fmt, percent_pos, specifier_pos, args);
	if (length_modifiers == "h")
		return convert_low_uint<uint16_t>(fmt, percent_pos, specifier_pos, args);
	if (length_modifiers.empty())
		return convert_high_uint<uint32_t>(fmt, percent_pos, specifier_pos, args);
	if (length_modifiers == "l" || length_modifiers == "ll")
		return convert_high_uint<uint64_t>(fmt, percent_pos, specifier_pos, args);
	return std::string::npos;
}

/**
 * @brief
 * Format the reply message corresponding to a given reply number.
 * Use the given variadic arguments to replace the conversion specifications in the raw reply message.
 *
 * @param reply_number The number of the reply to format.
 * @param ... The variadic arguments to use for the conversion specifications.
 *
 * @return The formatted reply message.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string format_reply(int const reply_number...)
{
	ReplyIterator const reply = replies.find(reply_number);

	if (reply == replies.end())
		throw UnknownReply();

	std::stringstream ss;

	ss << std::setw(3) << std::setfill('0') << reply_number << ' ' << reply->second;

	std::string fmt = ss.str();
	size_t      percent_pos = fmt.find('%');
	va_list     args;

	va_start(args, reply_number);
	while (percent_pos != std::string::npos)
	{
		size_t const      specifier_pos = fmt.find_first_not_of("hl", percent_pos + 1);
		std::string const length_modifiers = fmt.substr(percent_pos + 1, specifier_pos - percent_pos - 1);
		size_t            inserted_len;

		switch (fmt[specifier_pos])
		{
		case 'c':
			inserted_len = convert_c(fmt, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 's':
			inserted_len = convert_s(fmt, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 'S':
			inserted_len = convert_S(fmt, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case 'u':
			inserted_len = convert_u(fmt, length_modifiers, percent_pos, specifier_pos, args);
			break;
		case '%':
			if (!length_modifiers.empty())
			{
				va_end(args);
				throw InvalidConversion();
			}

			fmt.erase(percent_pos, 1);
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
		percent_pos = fmt.find('%', percent_pos + inserted_len);
	}
	va_end(args);

	return fmt;
}
// TODO: implement unit tests for this function
