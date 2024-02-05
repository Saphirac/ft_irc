/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:58:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/05 22:31:02 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Server instance, initializing its fields to be empty.
 *
 * @return The newly created Server instance.
 */
Server::Server(std::string const &servername) : _name(servername), _clients_by_socket(), _clients_by_nickname() {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Server::~Server(void) { this->_clients_by_socket.clear(); }

// ***************************************************************************************************************** //
//                                              Public Member Functions                                              //
// ***************************************************************************************************************** //
/**
 * @brief Copies a given Client instance to the list of known clients.
 *
 * @param client The Client instance to copy.
 */
void Server::add_client(Client const &client)
{
	std::pair<std::map<int, Client>::iterator, bool> ret =
		this->_clients_by_socket.insert(std::make_pair(client.get_socket(), client));

	if (ret.second)
		this->_clients_by_nickname.insert(std::make_pair(client.get_nickname(), &ret.first->second));
}

/**
 * @brief Removes a client from the list of known clients.
 *
 * @param client The Client instance to remove.
 */
void Server::remove_client(Client const &client)
{
	this->_clients_by_nickname.erase(client.get_nickname());
	this->_clients_by_socket.erase(client.get_socket());
}
