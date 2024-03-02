/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/02 03:24:49 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include "class/Hostname.hpp"
#include "class/Nickname.hpp"
#include "class/Realname.hpp"
#include "class/Server.hpp"
#include "class/UserModes.hpp"
#include "class/Username.hpp"
#include <ctime>
#include <string>

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

	epoll_event       *get_mut_epoll_event(void) const;
	std::clock_t       get_time_last_msg(void) const;
	std::string const &get_msg_in(void) const;
	std::string const &get_msg_out(void) const;
	
	Nickname const &get_nickname(void) const;
	Hostname const &get_hostname(void) const;
	Username const &get_username(void) const;
	Realname const &get_realname(void) const;
	
	UserModes            get_modes(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_msg_in(std::string const &messages);
	void set_msg_out(std::string const &messages);
	
	void set_nickname(Nickname const &nickname);
	void set_hostname(Hostname const &hostname);
	void set_username(Username const &username);
	void set_realname(Realname const &realname);
	
	void set_modes(UserModes const modes);
	void set_time_last_msg(void);
	void set_time_last_ping(void);

	// Member functions
	void disconnect(void);
	void append_to_msg_in(std::string const &s);
	void append_to_msg_out(std::string const &msg);
	void clear_msg_out(void);
	void set_mode(UserMode const mode);
	void clear_mode(UserMode const mode);
	void set_time_last_msg(void);

	bool has_mode(UserMode const mode) const;

	//ssize_t send_message(std::string const &message) const;
	//ssize_t send_messages(void) const;

	std::string user_mask(void) const;

	std::clock_t check_time_since_last_msg(void) const;

	void send_msg_out(void);

	std::string const get_next_msg(void);
	epoll_event      *set_epoll_event();
};
