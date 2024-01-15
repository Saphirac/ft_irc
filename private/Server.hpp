/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:45:32 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/15 16:03:19 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include <vector>
//# include "Client.hpp"
# include <sys/types.h>
# include <netinet/in.h>

class Client;
class Channel;

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

	std::string	_name;
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
	std::string const	&getName() const;
	int	getIp() const;
	int	getPort() const;
	int	getSocket() const;
	sockaddr_in const	&getSockAddr() const;
	std::string const	&getPassword() const;
	socklen_t const	&getSockLen() const;
	std::vector<Client*> const	&getClients() const;
	std::vector<Channel*> const	&getChannels() const;

	// Setters //
	void	setName(std::string const &name);
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
Server	*create_server(int chosen_addr, std::string password);


#endif
