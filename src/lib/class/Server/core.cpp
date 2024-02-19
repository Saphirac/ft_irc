/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 06:58:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 13:31:38 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include <ctime>

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Server instance, initializing its fields with the given arguments.
 *
 * @param name The name of the server.
 * @param version The version of the server.
 * @param password The password of the server (required to connect to it).
 *
 * @return The newly created Server instance.
 */
/*Server::Server(std::string const &name, std::string const &version, std::string const &password) :
	_name(name),
	_version(version),
	_password(password),
	_creation_date(),
	_creation_time(),
	_compilation_date(__DATE__),
	_compilation_time(__TIME__),
	_clients_by_socket(),
	_clients_by_nickname()
{
	time_t const raw_time = time(NULL);
	tm const    *time_info = localtime(&raw_time);
	char         buffer[80];

	strftime(buffer, sizeof(buffer), "%Y-%m-%d", time_info);
	this->_creation_date = buffer;
	strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info);
	this->_creation_time = buffer;
}*/

// ******************************************************************************************************************
// //
//                                                     Destructor //
// ******************************************************************************************************************
// //
Server::~Server(void) { this->_clients_socket.clear(); }

// ***************************************************************************************************************** //
//                                              Public Member Functions                                              //
// ***************************************************************************************************************** //
/**
 * @brief Copies a given Client instance to the list of known clients.
 *
 * @param client The Client instance to copy.
 */
/*void Server::add_client(Client const &client)
{
	this->_clients_socket[client.get_socket()] = client;

	if (ret.second)
		this->_clients_nick.insert(std::make_pair(client.get_nickname(), &ret.first->second));
}*/

/**
 * @brief Removes a client from the list of known clients.
 *
 * @param client The Client instance to remove.
 */
void Server::remove_client(Client const &client)
{
	this->_clients_nick.erase(client.get_nickname());
	this->_clients_socket.erase(client.get_socket());
}
