/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_reply.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 21:49:58 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/06 01:31:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replies.hpp"
#include <cstdarg>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>

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
 */
inline static size_t convert_c(
	std::string       &fmt,
	std::string const &length_modifiers,
	size_t const       percent_pos,
	size_t const       specifier_pos,
	va_list            args)
{
	if (length_modifiers == "l")
	{
		wchar_t const arg(static_cast<wchar_t>(va_arg(args, int)));
		int const     len_as_int = mblen(reinterpret_cast<char const *>(&arg), sizeof(wchar_t));

		if (len_as_int == -1)
		{
			fmt.erase(percent_pos, specifier_pos - percent_pos + 1);
			return 0;
		}

		size_t const len = static_cast<size_t>(len_as_int);

		fmt.replace(percent_pos, specifier_pos - percent_pos + 1, reinterpret_cast<char const *>(&arg), len);
		return len;
	}
	if (length_modifiers.empty())
	{
		char const arg(static_cast<char>(va_arg(args, int)));

		fmt.replace(percent_pos, specifier_pos - percent_pos + 1, 1, arg);
		return 1;
	}
	return std::string::npos;
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

	std::stringstream ss;

	ss << va_arg(args, char const *);

	std::string const arg(ss.str());

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

	std::string const arg(ss.str());

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

	std::string const arg(ss.str());

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

// TODO: implement unit tests for this function
/**
 * @brief
 * Format the reply message corresponding to a given reply number.
 * Use the given variadic arguments to replace the conversion specifications in the raw reply message.
 *
 * @param reply_number The number of the reply to format.
 * @param ... The variadic arguments to use for the conversion specifications.
 *
 * @return The formatted reply message, or an empty string if an error occured.
 */
std::string format_reply(int const reply_number...)
{
	ReplyIterator const reply = replies.find(reply_number);

	if (reply == replies.end())
	{
		std::cerr << "unknown reply number: " << std::setw(3) << std::setfill('0') << reply_number << '\n';
		return "";
	}

	std::stringstream ss;

	ss << std::setw(3) << std::setfill('0') << reply_number << ' ' << reply->second;

	std::string fmt(ss.str());
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
			case 'u':
				inserted_len = convert_u(fmt, length_modifiers, percent_pos, specifier_pos, args);
				break;
			case '%':
				if (!length_modifiers.empty())
				{
					std::cerr << "invalid conversion specification";
					return "";
				}
				fmt.erase(percent_pos, 1);
				inserted_len = 1;
				break;
			default:
				std::cerr << "invalid conversion specification";
				return "";
		}
		if (inserted_len == std::string::npos)
		{
			std::cerr << "invalid conversion specification";
			return "";
		}
		percent_pos = fmt.find('%', percent_pos + inserted_len);
	}
	va_end(args);

	return fmt;
}