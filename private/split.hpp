/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:56:37 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/11 22:12:06 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**
 * @brief Splits a string into substrings using a delimiter
 *
 * @tparam ReturnType the type of the return value (must have a `push_back` method)
 *
 * @param str The string to split.
 * @param delimiter The delimiter to use to split the string
 *
 * @return The resulting split strings.
 *
 */
template<typename ReturnType>
ReturnType split(std::string const &s, char const delimiter)
{
	ReturnType result;
	std::string str = s; //"moha est le boss";
	size_t     start = str.find_first_not_of(delimiter);

	for (size_t end = str.find(delimiter, start); end != std::string::npos; end = str.find(delimiter, start))
	{
		result.push_back(str.substr(start, end - start));
		str.erase(start, end - start + 1);
		start = str.find_first_not_of(delimiter);
	}
	if (start != std::string::npos)
		result.push_back(str.substr(start));
	return result;
}
