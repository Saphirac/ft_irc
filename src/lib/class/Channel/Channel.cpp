/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:04:21 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/02 00:48:35 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Channel.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel(std::string const &topic) : _topic(topic), _users_limit(0)
{
	if (DEBUG)
		std::cout << "Channel constructor called" << std::endl;
}

Channel::~Channel()
{
	if (DEBUG)
		std::cout << "Channel destructor called" << std::endl;
}

// Getters

std::string const Channel::get_topic(void) { return this->_topic; }
std::string const Channel::get_key(void) { return this->_key; }

std::map<std::string, Client *> const &Channel::get_clients(void) { return this->_clients; }
std::map<std::string, Client *> const &Channel::get_operators(void) { return this->_operators; }
std::map<std::string, Client *> const &Channel::get_invite_list(void) { return this->_invite_list; }
std::map<std::string, Client *> const &Channel::get_ban_list(void) { return this->_ban_list; }

std::vector<ChannelMode> Channel::get_modes(void) { return this->_modes; }
int                      Channel::get_users_limit(void) { return this->_users_limit; }

// Setters

void Channel::set_topic(std::string const &topic) { this->_topic = topic; }

void Channel::add_client(Client &client) { this->_clients[client.get_nickname()] = &client; }
void Channel::remove_client(Client &client) { this->_clients.erase(client.get_nickname()); }

void Channel::add_operator(Client &client) { this->_operators[client.get_nickname()] = &client; }
void Channel::remove_operator(Client &client) { this->_operators.erase(client.get_nickname()); }

void Channel::add_to_invite_list(Client &client) { this->_invite_list[client.get_nickname()] = &client; }
void Channel::remove_from_invite_list(Client &client) { this->_invite_list.erase(client.get_nickname()); }
void Channel::add_to_ban_list(Client &client) { this->_ban_list[client.get_nickname()] = &client; }
void Channel::remove_from_ban_list(Client &client) { this->_ban_list.erase(client.get_nickname()); }

void Channel::add_mode(ChannelMode mode) { this->_modes.push_back(mode); }
void Channel::remove_mode(ChannelMode mode)
{
	std::vector<ChannelMode>::iterator it = std::find(this->_modes.begin(), this->_modes.end(), mode);

	if (it != this->_modes.end())
		this->_modes.erase(it);
}

void Channel::set_limit(int users_limit) { _users_limit = users_limit; }
void Channel::set_key(std::string key) { _key = key; }

// Member functions

// TODO : Commented because I need to complete it with the prefix method of Client
/*void	Channel::send_msg_to_all(std::string const &msg, Client &sender)
{
    std::map<std::string, Client &>::iterator it = this->_clients.begin();
    while (it != this->_clients.end())
    {
        if (it.second != &sender)
            it.second->append_to_msg_out(msg);
        it++;
    }
}*/

/**
 * @brief send the message to all the clients in the channel
 *
 * @param msg to send
 */
void Channel::send_msg_to_all(std::string const &msg)
{
	std::map<std::string, Client *>::iterator it = this->_clients.begin();

	while (it != this->_clients.end())
	{
		it->second->append_to_msg_out(msg);
		it++;
	}
}
