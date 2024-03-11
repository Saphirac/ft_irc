/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:56:37 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/10 04:39:39 by jodufour         ###   ########.fr       */
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
ReturnType split(std::string const &str, char const delimiter)
{
	ReturnType             result;
	std::string::size_type start = str.find_first_not_of(delimiter);
	std::string::size_type end = start == std::string::npos ? std::string::npos : str.find(delimiter, start);

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
