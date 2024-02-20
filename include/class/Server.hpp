/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 19:26:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "StatusCode.hpp"
#include "class/Client.hpp"
#include <map>
#include <set>
#include <vector>

class Server
{
private:
	// Shared fields
	static std::set<std::string> const              _operator_hosts;
	static std::map<std::string, std::string> const _operator_ids;

	// Fields
	std::string                     _name;
	std::string                     _version;
	std::string                     _password;
	std::string                     _creation_date;
	std::string                     _creation_time;
	std::string                     _compilation_date;
	std::string                     _compilation_time;
	std::map<int, Client>           _clients_by_socket;
	std::map<std::string, Client *> _clients_by_nickname;

public:
	// Constructors
	Server(std::string const &name, std::string const &version, std::string const &password = "");

	// Destructor
	~Server(void);

	// Member functions
	void add_client(Client const &client);
	void remove_client(Client const &client);

	StatusCode away(Client &sender, std::vector<std::string> const &parameters);
	StatusCode nick(Client &sender, std::vector<std::string> const &parameters);
	StatusCode pass(Client &sender, std::vector<std::string> const &parameters);
	StatusCode oper(Client &sender, std::vector<std::string> const &parameters);
	StatusCode user(Client &sender, std::vector<std::string> const &parameters);
};
