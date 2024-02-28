/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 16:45:42 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Channel.hpp"
#include "class/Client.hpp"
#include <map>
#include <set>
#include <vector>

class Server
{
private:
	// Shared fields
	static std::set<std::string> const                    _operator_hosts;
	static std::map<std::string, std::string const> const _operator_ids;

	// Fields
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

public:
	// Constructors
	Server(std::string const &name, std::string const &version, std::string const &password = "");

	// Destructor
	~Server(void);

	// Methods
	void add_client(Client const &client);
	void remove_client(Client const &client);

	// Commands
	void away(Client &sender, std::vector<std::string> const &parameters);
	void mode(Client &sender, std::vector<std::string> const &parameters);
	void nick(Client &sender, std::vector<std::string> const &parameters);
	void pass(Client &sender, std::vector<std::string> const &parameters);
	void oper(Client &sender, std::vector<std::string> const &parameters);
	void user(Client &sender, std::vector<std::string> const &parameters);
};
