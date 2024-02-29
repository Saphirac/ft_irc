/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:04:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 17:45:56 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ChannelMode.hpp"
#include "class/Client.hpp"
#include "class/Server.hpp"

class Client;

class Channel
{
private:
	std::string _topic;
	std::string _key;

	std::map<std::string, Client *> _clients;
	std::map<std::string, Client *> _operators;
	std::map<std::string, Client *> _invite_list;
	std::map<std::string, Client *> _ban_list;

	std::vector<ChannelMode> _modes;

	int _users_limit;

public:
	Channel(std::string const &topic = "");
	~Channel();

	std::string const get_topic(void);
	std::string const get_key(void);

	std::map<std::string, Client *> const &get_clients(void);
	std::map<std::string, Client *> const &get_operators(void);
	std::map<std::string, Client *> const &get_invite_list(void);
	std::map<std::string, Client *> const &get_ban_list(void);

	std::vector<ChannelMode> get_modes(void);
	int                      get_users_limit(void);

	void set_topic(std::string const &topic);
	void add_client(Client &client);
	void add_operator(Client &client);
	void remove_client(Client &client);
	void remove_operator(Client &client);
	void add_mode(ChannelMode mode);
	void remove_mode(ChannelMode mode);
	void add_to_invite_list(Client &client);
	void remove_from_invite_list(Client &client);
	void add_to_ban_list(Client &client);
	void remove_from_ban_list(Client &client);
	void set_key(std::string key);
	void set_limit(int limit);

	void display_mode(void);

	// void send_msg_to_all(std::string const &msg, Client &sender);
	void send_msg_to_all(std::string const &msg);
};
