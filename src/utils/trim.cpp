/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:38:54 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/12/09 17:36:58 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cctype>
#include <iostream>

// Fonction pour élaguer les espaces blancs au début et à la fin d'une chaîne de caractères
std::string trim(std::string const &str)
{
	std::string::size_type start = str.find_first_not_of(" \t\r\n");
	std::string::size_type end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}
