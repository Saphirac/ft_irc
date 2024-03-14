/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:56:37 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 00:52:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
	std::string        str = s; //"moha est le boss";
	size_t             begin = str.find_first_not_of(delimiter);
	size_t             end = str.find_first_of(delimiter, begin);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(begin, end - begin));
		str.erase(begin, end - begin + 1);
		begin = str.find_first_not_of(delimiter);
		end = str.find(delimiter, begin);
	}
	if (begin != std::string::npos)
		result.push_back(str.substr(begin));
	return result;
}
