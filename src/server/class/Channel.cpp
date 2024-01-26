/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:04:21 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:05:40 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channels.hpp"

Channels::Channels(std::string name) : _name(name)
{
	if (DEBUG)
		std::cout << "Channels constructor called\n";
}

Channels::~Channels()
{
	if (DEBUG)
		std::cout << "Channels destructor called\n";
}

// Getters //

std::string Channels::getName(void)
{
	if (DEBUG)
		std::cout << "getName() member function of Channels called\n";
	return this->_name;
}

std::string Channels::getTopic(void)
{
	if (DEBUG)
		std::cout << "getTopic() member function of Channels called\n";
	return this->_topic;
}

std::vector<Client *> Channels::getClients(void)
{
	if (DEBUG)
		std::cout << "getClients() member function of Channels called\n";
	return this->_clients;
}

std::vector<Client *> Channels::getOperators(void)
{
	if (DEBUG)
		std::cout << "getOperators() member function of Channels called\n";
	return this->_operators;
}

// Setters //

void Channels::setName(std::string name)
{
	if (DEBUG)
		std::cout << "setName() member function of Channels called\n";
	this->_name = name;
}

void Channels::setTopic(std::string topic)
{
	if (DEBUG)
		std::cout << "setTopic() member function of Channels called\n";
	this->_topic = topic;
}

void Channels::setClients(std::vector<Client *> clients)
{
	if (DEBUG)
		std::cout << "setClients() member function of Channels called\n";
	this->_clients = clients;
}

void Channels::setOperators(std::vector<Client *> operators)
{
	if (DEBUG)
		std::cout << "setOperators() member function of Channels called\n";
	this->_operators = operators;
}

void Channels::addClient(Client *client)
{
	if (DEBUG)
		std::cout << "addClient() member function of Channels called\n";
	this->_clients.push_back(client);
}

void Channels::addOperator(Client *client)
{
	if (DEBUG)
		std::cout << "addOperator() member function of Channels called\n";
	this->_operators.push_back(client);
}

void Channels::removeClient(Client *client)
{
	if (DEBUG)
		std::cout << "removeClient() member function of Channels called\n";
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (*it == client)
		{
			this->_clients.erase(it);
			break ;
		}
	}
}
