/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:38:07 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/06 01:23:17 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "StatusCode.hpp"
#include "class/Client.hpp"
#include <map>
#include <vector>

class Server
{
private:
	// Fields
	std::string                     _name;
	std::map<int, Client>           _clients_by_socket;
	std::map<std::string, Client *> _clients_by_nickname;

public:
	// Constructors
	Server(std::string const &name = "");

	// Destructor
	~Server(void);

	// Member functions
	void add_client(Client const &client);
	void remove_client(Client const &client);

	StatusCode nick(Client &sender, std::string const &parameters);
	StatusCode pass(Client &sender, std::string const &parameters);
};

#endif