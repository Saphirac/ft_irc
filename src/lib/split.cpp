/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 16:06:35 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 18:10:36 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Split a string into a vector of strings using a delimiter
 *
 * @param str the string to split
 * @param delimiter the delimiter to use to split the string
 * @return the vector of strings
 */
std::vector<std::string> split(std::string const &str, char const delimiter)
{
	std::vector<std::string> result;
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
