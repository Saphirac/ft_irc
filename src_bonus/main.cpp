/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:13:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 18:56:14 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include <iostream>
#include <cstdlib>

int main(int const ac, char const *const *const av)
{
	if (ac < 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int         port = atoi(av[1]);
	std::string password = av[2];

	try 
	{
		Bot bot(port, password);
		bot.run();
	}
	catch (std::exception const &e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

