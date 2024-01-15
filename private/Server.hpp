/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:45:32 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/12/23 02:33:30 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include <vector>
//# include "Client.hpp"
# include <sys/types.h>
# include <netinet/in.h>

// Class //

/**
 * @brief class Server to contain everything I need for my server
 * _port, password etc.
 * 
 * @todo maybe choose a better type than int
 * 		do a proper default constructor if void
 */

class	Server
{
private:

	int	_ip;
	int	_port;
	int	_socket;
	sockaddr_in	_sock_addr;
	socklen_t	_addr_len;
	std::string	_password;
	std::vector<Client *>	_clients;
	std::vector<Channel *>	_channels;

public:

	Server(int const port, std::string const password);
	~Server();

	// Getters //
	int	getIp() const;
	int	getPort() const;
	int	getSocket() const;
	sockaddr_in const	&getSockAddr() const;
	std::string const	&getPassword() const;
	socklen_t const	&getSockLen() const;
	std::vector<Client*> const	&getClients() const;
	std::vector<Channel*> const	&getChannels() const;

	// Setters //
	void	setIp(int const ip);
	void	setPort(int const port);
	void	setSocket(int const socket);
	void	setSockAddr(sockaddr_in const &addr);
	void	setSockLen();
	void	setPassword(std::string const &password);
	void	setClients(std::vector<Client*> const &clients);
	void	setChannels(std::vector<Channel*> const &channels);
	
};

// Tests //
void	create_server(int chosen_addr);


#endif
