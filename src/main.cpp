/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 18:59:48 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "ft_irc.hpp"
#include <iostream>

// TODO : create a checker for the arguments
int main(int const ac, char const *const *const av)
{
	if (ac != 3)
	{
		std::cerr << "incorrect number of args\n";
		return 1;
	}

	Server *myserver = new Server(atoi(av[1]), std::string(av[2]), "MyServer");

	try
	{
		myserver->create_server();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
