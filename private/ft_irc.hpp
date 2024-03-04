/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 19:27:50 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef DEBUG
#	define DEBUG 0
#endif

#include <string>

// Utils //

std::string              format_reply(int const reply_number...);

/**
 * @brief Split a string into substrings using a delimiter
 *
 * @tparam ReturnType the type of the return value
 * @param str the string to split
 * @param delimiter the delimiter to use to split the string
 * @return the resulting split strings.
 * 
 */
template<typename ReturnType>
ReturnType split(std::string const &str, char const delimiter)
{
	ReturnType result;
	std::string::size_type   start = str.find_first_not_of(delimiter);
	std::string::size_type   end = start == std::string::npos ? std::string::npos : str.find(delimiter, start);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(start, end - start));
		start = str.find_first_not_of(delimiter, end + 1);
		end = start == std::string::npos ? std::string::npos : str.find(delimiter, start);
	}
	if (start != std::string::npos)
		result.push_back(str.substr(start));
	return result;
}
