/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:55:39 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/11/26 16:35:11 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <sys/socket.h>

/**
 * @brief Create a socket object
 * 
 * @todo create an exception to be thrown for more proper use
 * 
 * @return socket create / if failure exit with EXIT_FAILURE
 */

int	create_socket()
{
	int fd_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd_socket == -1)
	{
		std::cerr << "Failed to create socket. errno: " << errno << std::endl;
		exit (EXIT_FAILURE);
	}
	return fd_socket;
}

sockaddr_in	&bind_assign_sockaddr(int fd_socket)
{
	sockaddr_in	sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(9999); 
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd_socket, (struct sock_addr *)&sock_addr, sizeof(sock_addr)) == -1)
	{
		std::cerr << "Failed to bind address. errno: " << errno << std::endl;
		exit (EXIT_FAILURE);
	}
	return (struct sock_addr *)&sock_addr;
}

int	create_server()
{
	
}
