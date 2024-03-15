/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:56:37 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 14:11:01 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

/**
 * @brief Splits a string into substrings using a delimiter
 *
 * @tparam SequenceCollection the type of the return value (must have a `push_back` method)
 *
 * @param s The string to split.
 * @param delimiter The delimiter to use to split the string
 *
 * @return The resulting split strings.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
template<typename SequenceCollection>
SequenceCollection split(std::string const &s, char const delimiter)
{
	SequenceCollection result;
	size_t             begin = s.find_first_not_of(delimiter);
	size_t             end = s.find_first_of(delimiter, begin);

	while (begin != std::string::npos)
	{
		result.push_back(s.substr(begin, end - begin));
		begin = s.find_first_not_of(delimiter, end);
		end = s.find_first_of(delimiter, begin);
	}

	return result;
}
