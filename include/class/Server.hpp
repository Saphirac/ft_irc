/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 18:58:36 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "StatusCode.hpp"
#include "class/Exceptions.hpp"
#include "ft_irc.hpp"
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <set>
#include <stdlib.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Client;
class Channel;

// TODO : change this to a proper value

#define MAX_CLIENTS 3

// Class //

/**
 * @brief class Server to contain everything I need for my server
 * _port, password etc.
 *
 * @todo maybe choose a better type than int and std::string for client
 * 		do a proper default constructor if void
 */

// define pointer on function for cmd:

class Server
{
private:
	// Shared fields
	static std::set<std::string> const              _operator_hosts;
	static std::map<std::string, std::string> const _operator_ids;

	// Typedef
	typedef StatusCode (Server::*Cmd)(Client &sender, std::vector<std::string> const &parameters);

	// Fields
	int                 _port;
	int                 _socket;
	int                 _epoll_socket;
	sockaddr_in         _sock_addr;
	socklen_t           _sock_len;
	struct epoll_event *_epoll_event;
	std::string         _name;
	std::string         _version;
	std::string         _password;
	std::string         _creation_date;
	std::string         _creation_time;
	std::string         _compilation_date;
	std::string         _compilation_time;

	// TODO : get back to a non pointer (hopefully)
	std::map<int, Client *>         _clients_socket;
	std::map<std::string, Client *> _clients_nick;
	std::vector<Channel *>          _channels;
	std::map<std::string, Cmd>      _map_of_cmds;

	bool _shutdown;

public:
	// Constructors
	Server(int const port, std::string const name, std::string const password, bool shutdown = false);
	// Destructor
	~Server(void);

	// Getters //
	int get_port() const;
	int get_socket() const;
	int get_epoll_socket() const;

	std::string const &get_name() const;
	std::string const &get_password() const;
	std::string const &get_creation_date() const;
	std::string const &get_creation_time() const;
	std::string const &get_compilation_date() const;
	std::string const &get_compilation_time() const;

	struct epoll_event       *get_epoll_event() const;
	struct sockaddr_in const &get_sock_addr() const;
	socklen_t const          &get_sock_len() const;

	std::vector<Channel *> const          &get_channels() const;
	std::map<int, Client *> const         &get_clients_socket() const;
	std::map<std::string, Client *> const &get_clients_nick() const;

	bool get_shutdown() const;

	// Setters //
	void set_port(int const port);
	void set_socket(int const socket);
	void set_epoll_socket(int const epoll_socket);

	void set_name(std::string const &name);
	void set_creation_date(std::string const &creation_date);
	void set_creation_time(std::string const &creation_time);
	void set_compilation_date(std::string const &compilation_date);
	void set_compilation_time(std::string const &compilation_time);
	void set_password(std::string const &password);

	void set_sock_addr(struct sockaddr_in const &sock_addr);
	void set_sock_len();
	void set_epoll_event();

	void set_clients_socket(std::map<int, Client *> const &clients_socket);
	void set_clients_nick(std::map<std::string, Client *> const &clients_nick);
	void set_channels(std::vector<Channel *> const &channels);

	void set_shutdown(bool shutdown);

	// Member functions
	// void add_client(Client *client);
	// void remove_client(Client *client);

	// Commands

	StatusCode cap(Client &sender, std::vector<std::string> const &parameters);
	StatusCode join(Client &sender, std::vector<std::string> const &parameters);
	StatusCode away(Client &sender, std::vector<std::string> const &parameters);
	StatusCode nick(Client &sender, std::vector<std::string> const &parameters);
	StatusCode pass(Client &sender, std::vector<std::string> const &parameters);
	StatusCode oper(Client &sender, std::vector<std::string> const &parameters);
	StatusCode user(Client &sender, std::vector<std::string> const &parameters);

	// Others
	void create_and_set_socket();
	void init_socket_server();
	void create_server();
	void ctrl_epoll_add(int epoll_fd, int socket, epoll_event *e_event);
	void handle_client_event(Client *client, std::string msg);
	void handle_new_connection();
	void epoll_loop();
	void handle_all_events_routine();
	void rcv_client_event(Client *client);
	void send_msg_out();

	struct sockaddr_in bind_assign_sockaddr();

	void init_map_cmd(void);

	void add_client(Client *client);
	void remove_client(Client *client);
};

int  create_epoll();
