/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:54:08 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/21 21:03:58 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class	Server;

class Client
{
private:

	int	_socket;
	std::string	_name;
	std::string	_nickname;
	sockaddr_in	_sock_addr;
	socklen_t	_addr_len;
	struct epoll_event	*_client_event;
	
public:

	Client(int const socket, sockaddr_in sock_addr, int const name, int const nickname);
	~Client();

	int	getSocket(void) const;
	std::string	getName(void) const;
	std::string	getNickname(void) const;
	struct epoll_event	*getEvent(void) const;

	void	setSocket(int const socket);
	void	setName(std::string const name);
	void	setNickname(std::string const nickname);
	void	setEvent();
};

#endif
