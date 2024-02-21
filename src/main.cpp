/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/20 21:10:32 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <cstdlib>
#include "class/Server.hpp"
#include <cstdio>
#include <stdio.h>

/**
 * @brief Launch irc server
 * 
 * // TODO : create a checker for the arguments
 * // TODO : create a catcher for exceptions
 * @param ac 
 * @param av 
 * @return int 
 */

int main(int ac, char **av)
{
	if (ac != 3)
		std::cout << "incorrect number of args\n";
	Server *myserver = new Server(atoi(av[1]), std::string(av[2]), "MyServer");
	try {
		myserver->create_server();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
