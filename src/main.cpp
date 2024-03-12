/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/12 05:42:02 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "class/specialized_string/Password.hpp"
#include <cstdlib>
#include <iostream>
#include <cctype>

inline static bool check_main_arguments(int const ac, char const *const *const av)
{
	if (ac != 3)
	{
		std::cerr << "incorrect: number of arguments" << std::endl;
		return false;
	}
	for (const char *p = av[1]; *p; ++p)
	{
		if (!isdigit(*p))
		{
			std::cerr << "incorrect: port must be only digit" << std::endl;
			return false;
		}
	}
	int const port = atoi(av[1]);
	if (!(port >= 1024 && port <= 49151))
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
