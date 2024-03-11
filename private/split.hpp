/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:56:37 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/11 07:55:34 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
	size_t     start = str.find_first_not_of(delimiter);

	for (size_t end = str.find(delimiter, start); end != std::string::npos; end = str.find(delimiter, end))
	{
		result.push_back(str.substr(start, end - start));
		start = str.find_first_not_of(delimiter, end + 1);
	}
	if (start != std::string::npos)
		result.push_back(str.substr(start));
	return result;
}
