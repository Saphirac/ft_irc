/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/12 03:27:31 by jodufour         ###   ########.fr       */
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
		Server(atoi(av[1]), "PharmacIRC", std::string(av[2])).start();
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
