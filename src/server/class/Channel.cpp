/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:04:21 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/06 01:10:57 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name)
{
	if (DEBUG)
		std::cout << "Channel constructor called\n";
}

Channel::~Channel()
{
	if (DEBUG)
		std::cout << "Channel destructor called\n";
}

// Getters //

std::string Channel::getName(void)
{
	if (DEBUG)
		std::cout << "getName() member function of Channel called\n";
	return this->_name;
}

std::string Channel::getTopic(void)
{
	if (DEBUG)
		std::cout << "getTopic() member function of Channel called\n";
	return this->_topic;
}

std::vector<Client *> Channel::getClients(void)
{
	if (DEBUG)
		std::cout << "getClients() member function of Channel called\n";
	return this->_clients;
}

std::vector<Client *> Channel::getOperators(void)
{
	if (DEBUG)
		std::cout << "getOperators() member function of Channel called\n";
	return this->_operators;
}

// Setters //

void Channel::setName(std::string name)
{
	if (DEBUG)
		std::cout << "setName() member function of Channel called\n";
	this->_name = name;
}

void Channel::setTopic(std::string topic)
{
	if (DEBUG)
		std::cout << "setTopic() member function of Channel called\n";
	this->_topic = topic;
}

void Channel::setClients(std::vector<Client *> clients)
{
	if (DEBUG)
		std::cout << "setClients() member function of Channel called\n";
	this->_clients = clients;
}

void Channel::setOperators(std::vector<Client *> operators)
{
	if (DEBUG)
		std::cout << "setOperators() member function of Channel called\n";
	this->_operators = operators;
}

void Channel::addClient(Client *client)
{
	if (DEBUG)
		std::cout << "addClient() member function of Channel called\n";
	this->_clients.push_back(client);
}

void Channel::addOperator(Client *client)
{
	if (DEBUG)
		std::cout << "addOperator() member function of Channel called\n";
	this->_operators.push_back(client);
}

void Channel::removeClient(Client *client)
{
	if (DEBUG)
		std::cout << "removeClient() member function of Channel called\n";
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (*it == client)
		{
			this->_clients.erase(it);
			break;
		}
	}
}
