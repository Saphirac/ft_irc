/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/14 00:13:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Client.hpp"
#include <netinet/in.h>
#include <vector>

// TODO : change this to a proper value

#define DEFAULT_PART_TEXT         "Leaving, bye! <3"
#define DEFAULT_QUIT_TEXT         "Disconnecting, bye! <3"
#define MAXIMUM_NUMBER_OF_CLIENTS 3

class Server
{
public:
	// Types
	typedef std::map<NickName, Client *const> ClientMap;
	typedef std::map<ChannelName, Channel>    ChannelMap;
	typedef std::vector<std::string>          CommandParameterVector;

	// Constructors
	Server(int const port, std::string const &name, std::string const &password = "");

	// Destructor
	~Server(void);

	// Methods
	void start(void);

private:
	// Types
	typedef void (Server::*_Command)(Client &sender, CommandParameterVector const &parameters);
	typedef std::set<std::string>                        _OperatorHostSet;
	typedef std::map<std::string, std::string const>     _OperatorIdMap;
	typedef std::pair<std::string const, _Command const> _CommandPair;
	typedef std::map<std::string, _Command const>        _CommandMap;

	// Shared fields
	static _OperatorHostSet const _operator_hosts;
	static _OperatorIdMap const   _operator_ids;
	static _CommandPair const     _raw_commands_by_name[];
	static _CommandMap const      _commands_by_name;

	// Fields
	int const         _socket;
	int const         _epoll_socket;
	sockaddr_in const _sock_addr;

	std::string const _name;
	std::string const _version;
	std::string const _password;
	std::string const _compilation_date;
	std::string const _compilation_time;
	std::string const _creation_date;
	std::string const _creation_time;

	std::map<int, Client> _clients_by_socket;
	ClientMap             _clients_by_nickname;
	ChannelMap            _channels_by_name;

	// Methods
	void _handle_epoll_events(void);
	void _new_client_connection(void);
	void _receive_data_from_client(Client &client);
	void _compute_next_msg_for_a_client(Client &client);
	void _check_time_of_last_msg(Client &client) const;
	void _remove_client(Client &client, std::string const &part_msg = DEFAULT_QUIT_TEXT);

	void _welcome(Client &client) const;
	void _make_client_leave_all_their_joined_channels(Client &client, std::string const &part_msg = DEFAULT_PART_TEXT);

	// Commands
	void _away(Client &sender, CommandParameterVector const &parameters);
	void _cap(Client &sender, CommandParameterVector const &parameters);
	void _list(Client &sender, CommandParameterVector const &parameters);
	void _mode(Client &sender, CommandParameterVector const &parameters);
	void _names(Client &sender, CommandParameterVector const &parameters);
	void _nick(Client &sender, CommandParameterVector const &parameters);
	void _oper(Client &sender, CommandParameterVector const &parameters);
	void _pass(Client &sender, CommandParameterVector const &parameters);
	void _ping(Client &sender, CommandParameterVector const &parameters);
	void _pong(Client &sender, CommandParameterVector const &parameters);
	void _quit(Client &sender, CommandParameterVector const &parameters);
	void _user(Client &sender, CommandParameterVector const &parameters);
	void _join(Client &sender, CommandParameterVector const &parameters);
	void _part(Client &sender, CommandParameterVector const &parameters);
	void _topic(Client &sender, CommandParameterVector const &parameters);
	void _invite(Client &sender, CommandParameterVector const &parameters);
	void _kick(Client &sender, CommandParameterVector const &parameters);
	void _privmsg(Client &sender, CommandParameterVector const &parameters);
	void _notice(Client &sender, CommandParameterVector const &parameters);
};
