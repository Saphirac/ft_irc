/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:32:25 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Client.hpp"
#include <map>
#include <netinet/in.h>
#include <set>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>

// class Client;
// class Channel;

// TODO : change this to a proper value

#define MAX_CLIENTS 3

// Class //

/**
 * @brief class Server to contain everything I need for my server
 * _port, password etc.
 */
class Server
{
public:
	// Constructors
	Server(int const port, std::string const &name, std::string const &password = "");

	// Destructor
	~Server(void);

	// Methods
	void start(void);

private:
	// Types
	typedef void (Server::*Command)(Client &sender, std::vector<std::string> const &parameters);
	typedef std::pair<std::string const, Command const> CommandPair;
	typedef std::map<std::string, Command const>        CommandMap;
	typedef CommandMap::const_iterator                  CommandIterator;

	// Shared fields
	static std::set<std::string> const                       _operator_hosts;
	static std::map<std::string, std::string const> const    _operator_ids;
	static std::pair<std::string const, Command const> const _raw_commands_by_name[];
	static CommandMap const                                  _commands_by_name;

	// Fields
	int         _socket;
	sockaddr_in _sock_addr;
	socklen_t   _sock_len;
	int         _epoll_socket;

	bool _shutdown;

	std::string                       _name;
	std::string                       _version;
	std::string                       _password;
	std::string                       _creation_date;
	std::string                       _creation_time;
	std::string                       _compilation_date;
	std::string                       _compilation_time;
	std::map<int, Client>             _clients_by_socket;
	std::map<NickName, Client *const> _clients_by_nickname;
	std::map<ChannelName, Channel>    _channels_by_name;

	// Methods
	void _add_client(Client const &client);
	void _remove_client(Client const &client);

	void _handle_epoll_events(void);
	void _compute_next_msg_for_each_client(void);
	void _new_client_connection(void);
	void _receive_data_from_client(Client &client);

	// Commands
	void _away(Client &sender, std::vector<std::string> const &parameters);
	void _cap(Client &sender, std::vector<std::string> const &parameters);
	void _mode(Client &sender, std::vector<std::string> const &parameters);
	void _nick(Client &sender, std::vector<std::string> const &parameters);
	void _oper(Client &sender, std::vector<std::string> const &parameters);
	void _pass(Client &sender, std::vector<std::string> const &parameters);
	void _quit(Client &sender, std::vector<std::string> const &parameters);
	void _user(Client &sender, std::vector<std::string> const &parameters);
};
