/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 16:45:32 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:36:00 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include <vector>
# include "Client.hpp"
# include <sys/types.h>
# include <netinet/in.h>
#include <map>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <unistd.h>

class Client;
class Channel;

// TODO : change this to a proper value

# define MAX_CLIENTS 3

// Class //

/**
 * @brief class Server to contain everything I need for my server
 * _port, password etc.
 * 
 * @todo maybe choose a better type than int and std::string for client
 * 		do a proper default constructor if void
 */

class	Server
{
private:

	int	_port;
	std::string	_name;
	int	_ip;
	int	_socket;
	sockaddr_in	_sock_addr;
	socklen_t	_addr_len;
	std::string	_password;
	std::vector<Client *>	_clients;
	std::map<int, Client *>	_client_socket;
	std::map<std::string, Client *>	_client_nick;
	std::vector<Channel *>	_channels;
	int	_epoll_socket;
	struct epoll_event	*_epoll_event;
	bool	_shutdown;
	
	class ProblemWithSocket : public std::exception {
	public:
	virtual char const *what(void) const throw() {
		return "Problem with socket"; }
	};

	class ProblemWithSockAddr : public std::exception {
	public:
	virtual char const *what(void) const throw() {
		return "Problem with SockAddr structure creation / assignation"; }
	};

public:

	Server(int const port, std::string const name, std::string const password);
	~Server();

	// Getters //
	std::string const	&getName() const;
	int	getIp() const;
	int	getPort() const;
	int	getSocket() const;
	struct sockaddr_in const	&getSockAddr() const;
	std::string const	&getPassword() const;
	socklen_t const	&getSockLen() const;
	std::vector<Client*> const	&getClients() const;
	std::vector<Channel*> const	&getChannels() const;
	int	getEpollSocket() const;
	struct epoll_event	*getEpollEvent() const;
	bool	getShutdown() const;

	// Setters //
	void	setName(std::string const &name);
	void	setIp(int const ip);
	void	setPort(int const port);
	void	setSocket(int const socket);
	void	setSockAddr(struct sockaddr_in const &addr);
	void	setSockLen();
	void	setPassword(std::string const &password);
	void	setClients(std::vector<Client*> const &clients);
	void	setChannels(std::vector<Channel*> const &channels);
	void	setEpollSocket(int fd);
	void	setEpollEvent();
	void	setShutdown(bool const yesno);

	// Others

	void	create_and_set_socket();
	struct sockaddr_in	bind_assign_sockaddr();
	void	init_server();
	void	create_server();
	void	ctrl_epoll_add(int epoll_fd, int socket, struct epoll_event *e_event);
	void	handle_client_event(Client *client);
	void	handle_new_connection();
};

int		create_epoll();

#endif
