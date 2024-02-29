/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 18:08:22 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "StatusCode.hpp"
#include "UserMode.hpp"
#include "class/Hostname.hpp"
#include "class/Nickname.hpp"
#include "class/Realname.hpp"
#include "class/Server.hpp"
#include "class/UserModes.hpp"
#include "class/Username.hpp"
#include <ctime>
#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include <unistd.h>

class Server;

class Server;

class Client
{
private:
	// Fields
	int         _socket;
	std::string _msg_in;
	std::string _msg_out;

	Nickname  _nickname;
	Hostname  _hostname;
	Username  _username;
	Realname  _realname;
	UserModes _modes;

	std::string _away_msg;

	struct epoll_event *_epoll_event;

	std::clock_t _time_last_msg;

public:
	// Shared fields
	static std::string const _default_away_msg;

	// Constructors
	Client(
		int const       socket = -1,
		Nickname const &nickname = Nickname(),
		Hostname const &hostname = Hostname(),
		Username const &username = Username(),
		Realname const &realname = Realname(),
		UserModes const modes = UserModes());
	Client(Client const &src);

	// Destructor
	~Client(void);

	// Accessors
	int get_socket(void) const;

	epoll_event       *get_epoll_event(void) const;
	std::clock_t       get_time_last_msg(void) const;
	std::string const &get_msg_in(void) const;
	std::string const &get_msg_out(void) const;
	Nickname const    &get_nickname(void) const;
	Hostname const    &get_hostname(void) const;
	Username const    &get_username(void) const;
	Realname const    &get_realname(void) const;
	UserModes          get_modes(void) const;
	std::string const &get_away_msg(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_msg_in(std::string const &msg_in);
	void set_msg_out(std::string const &msg_out);
	void set_nickname(Nickname const &nickname);
	void set_hostname(Hostname const &hostname);
	void set_username(Username const &username);
	void set_realname(Realname const &realname);
	void set_modes(UserModes const modes);
	void set_away_msg(std::string const &away_msg);
	void set_epoll_event();

	// Member functions
	void disconnect(void);
	void append_to_msg_in(std::string const &s);
	void append_to_msg_out(std::string const &msg);
	void clear_msg_out(void);
	void set_mode(UserMode const mode);
	void clear_mode(UserMode const mode);
	void set_time_last_msg(void);

	bool has_mode(UserMode const mode) const;

	std::string user_mask(void) const;

	std::clock_t check_time_since_last_msg(void) const;

	void send_msg_out(void);

	std::string const get_next_msg(void);
};
