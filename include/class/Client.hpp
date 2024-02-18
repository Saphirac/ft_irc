/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 00:12:47 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Hostname.hpp"
#include "class/Nickname.hpp"
#include "class/Realname.hpp"
#include "class/UserModes.hpp"
#include "class/Username.hpp"
#include <stdint.h>
#include <string>
#include <unistd.h>

class Client
{
private:
	// Fields
	int         _socket;
	std::string _messages;

	Nickname  _nickname;
	Hostname  _hostname;
	Username  _username;
	Realname  _realname;
	UserModes _modes;

public:
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
	int                get_socket(void) const;
	std::string const &get_messages(void) const;
	Nickname const    &get_nickname(void) const;
	Hostname const    &get_hostname(void) const;
	Username const    &get_username(void) const;
	Realname const    &get_realname(void) const;
	UserModes          get_modes(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_messages(std::string const &messages);
	void set_nickname(Nickname const &nickname);
	void set_hostname(Hostname const &hostname);
	void set_username(Username const &username);
	void set_realname(Realname const &realname);
	void set_modes(UserModes const modes);

	// Methods
	void disconnect(void);
	void append_message(std::string const &message);
	void clear_messages(void);
	void set_mode(UserMode const mode);
	void clear_mode(UserMode const mode);

	bool has_mode(UserMode const mode) const;

	ssize_t send_messages(void) const;

	std::string user_mask(void) const;
};
