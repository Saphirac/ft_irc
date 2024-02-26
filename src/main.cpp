/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/28 16:20:55 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstdlib>
#include "Server.hpp"
#include <cstdio>
#include "IrcMessage.hpp"

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
	if (ac != 4)
		return 1;
	IrcMessage ircmsg;
	ircmsg = ircmsg.parseIrcMessage(std::string(av[3]));
	ircmsg.display();
	Server *myserver = new Server(atoi(av[1]), std::string(av[2]), "MyServer");
	try {
		myserver->init_server();
		myserver->create_server();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	printf("hi\n");
	return 0;
}
