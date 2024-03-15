/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_main_arguments.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 01:06:56 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/15 05:11:21 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Password.hpp"
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <iostream>

#define MINIMUM_PORT 1024
#define MAXIMUM_PORT 49151

/**
 * @brief check if the main arguments are correct
 *
 * The server can only use ports with numbers 1024-49151 because that are registered ports;
 *
 * @param ac number of arguments
 * @param av arguments
 * @return true if the arguments are correct
 */
bool check_main_arguments(int const ac, char const *const *const av)
{
	if (ac != 3)
	{
		std::cerr << "incorrect: number of arguments" << std::endl;
		return false;
	}

	char          *end_ptr;
	long int const port = strtol(av[1], &end_ptr, 10);

	errno = 0;
	if (errno == ERANGE)
	{
		std::cerr << "incorrect: port conversion error" << std::endl;
		return false;
	}

	if (*end_ptr || (port < MINIMUM_PORT || port > MAXIMUM_PORT))
	{
		std::cerr << "incorrect: port must be a number composed of only digits between " << MINIMUM_PORT << " and " << MAXIMUM_PORT << std::endl;
		return false;
	}

	if (!Password(av[2]).is_valid())
	{
		std::cerr << "incorrect: password does not respect correct notation" << std::endl;
		return false;
	}

	return true;
}
