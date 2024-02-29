/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:06:31 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 18:41:08 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

// Init epoll //

/**
 * @brief Create an epoll object with epoll_create1()
 * in case of error, exit with EXIT_FAILURE
 *
 * @return the created epoll fd
 */
int create_epoll()
{
	int epoll_fd = epoll_create1(0);

	if (epoll_fd == -1)
		throw ProblemWithEpollCreate();
	return epoll_fd;
}

/**
 * @brief add to epoll list of controlled fd the server socket
 *
 * @param epoll_fd the epoll fd containing the list of controlled fd
 * @param socket new socket to add to the list
 * @param e_event the epoll_event to add to the list
 */
void Server::ctrl_epoll_add(int epoll_fd, int socket, epoll_event *e_event)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, e_event) == -1)
		throw ProblemWithEpollCtl();
}
