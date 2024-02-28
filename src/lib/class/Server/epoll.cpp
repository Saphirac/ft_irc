/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:06:31 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:43:15 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

// Init epoll //

/**
 * @brief Create an epoll object with epoll_create1()
 * in case of error, exit with EXIT_FAILURE
 * 
 * // TODO : change exit failure with exception
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
 * TODO : change exit failure with exception
 * @param epoll_fd
 * @param myserver
 */
void Server::ctrl_epoll_add(int epoll_fd, int socket, struct epoll_event *e_event)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, e_event) == -1)
		throw ProblemWithEpollCtl();
}
