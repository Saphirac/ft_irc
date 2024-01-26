/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:11:37 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstdlib>
#include "Server.hpp"

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
		return 1;
	Server *myserver = new Server(atoi(av[1]), std::string(av[2]), "MyServer");
	myserver->init_server();
	myserver->create_server();
	return 0;
}
