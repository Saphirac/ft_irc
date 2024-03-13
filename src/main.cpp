/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/13 12:37:19 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "class/specialized_string/Password.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <climits>

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
inline static bool check_main_arguments(int const ac, char const *const *const av)
{
	if (ac != 3)
	{
		std::cerr << "incorrect: number of arguments" << std::endl;
		return false;
	}
	for (const char *it = av[1]; *it; ++it)
	{
		if (!isdigit(*it))
		{
			std::cerr << "incorrect: port must be only digit" << std::endl;
			return false;
		}
	}

	char *end_ptr;
	errno = 0;
	long int const port = strtol(av[1], &end_ptr, 10);

	if ((errno == ERANGE && (port == LONG_MAX || port == LONG_MIN)) || (errno != 0 && port == 0))
	{
		std::cerr << "incorrect: port conversion error" << std::endl;
		return false;
	}
	
	if (*end_ptr != '\0' || !(port >= MINIMUM_PORT && port <= MAXIMUM_PORT))
	{
		std::cerr << "incorrect: port must be between 1024 and 49151" << std::endl;
		return false;
	}

	if (!Password(av[2]).is_valid())
	{
		std::cerr << "incorrect: password does not respect correct notation" << std::endl;
		return false;
	}

	return true;
}

int main(int const ac, char const *const *const av)
{
	srand(time(NULL));

	if (!check_main_arguments(ac, av))
		return 1;

	try
	{
		Server(atoi(av[1]), "MyServer", std::string(av[2])).start();
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
