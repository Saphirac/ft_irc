/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/04 05:09:56 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdint.h>
#include <string>
#include <unistd.h>

class Client
{
private:
	// Fields
	int _socket;

	std::string _nickname;
	std::string _hostname;
	std::string _username;
	std::string _realname;
	uint8_t     _modes;

public:
	// Constructors
	Client(
		int const          socket = -1,
		std::string const &nickname = "",
		std::string const &hostname = "",
		std::string const &username = "",
		std::string const &realname = "",
		uint8_t const      modes = 0);

	// Destructor
	~Client(void);

	// Accessors
	int                get_socket(void) const;
	std::string const &get_nickname(void) const;
	std::string const &get_hostname(void) const;
	std::string const &get_username(void) const;
	std::string const &get_realname(void) const;
	uint8_t            get_modes(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_nickname(std::string const &nickname);
	void set_hostname(std::string const &hostname);
	void set_username(std::string const &username);
	void set_realname(std::string const &realname);
	void set_modes(uint8_t const modes);

	// Member functions
	void disconnect(void);

	bool has_already_sent_pass(void) const;

	ssize_t send_message(std::string const &message) const;
};

#endif