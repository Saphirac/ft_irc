/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:45:32 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/20 19:45:44 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include <vector>
//# include "Client.hpp"
# include <sys/types.h>
# include <netinet/in.h>
#include <map>

class Client;
class Channel;

// TODO : change this to a proper value

# define MAX_CLIENTS 3

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
	std::map<int, Client *>	_client_socket;
	std::map<std::stringz, Client *>	_client_nick;
	std::vector<Channel *>	_channels;
	int	_epoll_socket;
	struct epoll_event	*_epoll_event;
	
	class ProblemWithSocket : public std::exception {
	public:
	virtual const char* what() const noexcept override {
		return "Problem with socket"; }
	};

	class ProblemWithSockAddr : public std::exception {
	public:
	virtual const char* what() const noexcept override {
		return "Problem with SockAddr structure creation / assignation"; }
	};

public:

	Server(int const port, std::string const password, std::string const name);
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
	int	getEpollSocket() const;
	struct epoll_event	*getEpollEvent() const;

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
	void	setEpollSocket(int fd);
	void	setEpollEvent();

	// Others

	int	create_and_set_socket();
	sockaddr_in	bind_assign_sockaddr();
	void	init_server();
	
};

// Tests //
Server	*create_server(int chosen_addr, std::string password);


#endif
