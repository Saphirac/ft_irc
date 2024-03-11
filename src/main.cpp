/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/11 16:52:29 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <cstdlib>
#include <iostream>

// TODO : create a checker for the arguments
int main(int const ac, char const *const *const av)
{
	srand(time(NULL));
	
	if (ac != 3)
	{
		std::cerr << "incorrect number of arguments" << std::endl;
		return 1;
	}

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
