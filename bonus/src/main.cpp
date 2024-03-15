/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:13:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/15 01:14:12 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include "libircserv.hpp"
#include <iostream>
#include <cstdlib>

int main(int const ac, char const *const *const av)
{
	if (!check_main_arguments(ac, av))
		return 1;

	try 
	{
		Bot bot(atoi(av[1]), std::string(av[2]));
		bot.run();
	}
	catch (std::exception const &e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

