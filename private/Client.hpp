/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:54:08 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/20 16:41:35 by mcourtoi         ###   ########.fr       */
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
	struct epoll_event	*_client_event;
	
public:
	Client(int const socket, int const name, int const nickname);
	~Client();

	int	getSocket(void) const;
	std::string	getName(void) const;
	std::string	getNickname(void) const;

	void	setSocket(int const socket);
	void	setName(std::string const name);
	void	setNickname(std::string const nickname);
};

#endif
