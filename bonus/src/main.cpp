/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:13:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/15 05:37:23 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/WallE.hpp"
#include "libircserv.hpp"
#include <cstdlib>
#include <iostream>

int main(int const ac, char const *const *const av)
{
	if (!check_main_arguments(ac, av))
		return 1;

	try
	{
		WallE bot(atoi(av[1]), std::string(av[2]));
		bot.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
