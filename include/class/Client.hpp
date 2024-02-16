/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/17 18:36:58 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "UserMode.hpp"
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "class/Server.hpp"
#include <netinet/in.h>

class Server;

class Client
{
private:
	// Fields
	int         _socket;
	std::string _messages;

	std::string _nickname;
	std::string _hostname;
	std::string _username;
	std::string _realname;
	uint8_t     _modes;

	struct sockaddr_in	*_sock_addr;
	socklen_t			_addr_len;

	struct epoll_event	*_epoll_event;
	
	bool	_is_complete;
	bool	_is_msg_complete;
	bool	_pass;

public:
	// Constructors
	Client(
		int const          socket = -1,
		std::string const &nickname = "",
		std::string const &hostname = "",
		std::string const &username = "",
		std::string const &realname = "",
		struct sockaddr_in *sock_addr = NULL,
		uint8_t const      modes = 0);
	Client(Client const &src);

	// Destructor
	~Client(void);

	// Accessors
	int                get_socket(void) const;
	std::string const &get_messages(void) const;
	std::string const &get_nickname(void) const;
	std::string const &get_hostname(void) const;
	std::string const &get_username(void) const;
	std::string const &get_realname(void) const;
	uint8_t            get_modes(void) const;
	
	struct epoll_event	*get_epoll_event(void) const;
	struct sockaddr_in	*get_sock_addr(void) const;
	socklen_t			get_sock_len(void) const;

	bool	get_is_complete() const;
	bool	get_is_msg_complete() const;
	bool	get_is_pass() const;

	// Mutators
	void set_socket(int const socket);
	void set_messages(std::string const &messages);
	void set_nickname(std::string const &nickname);
	void set_hostname(std::string const &hostname);
	void set_username(std::string const &username);
	void set_realname(std::string const &realname);
	void set_modes(uint8_t const modes);

	void	set_epoll_event();
	void	set_is_complete(bool const yesno);
	void	set_is_msg_complete(bool const yesno);
	void	set_is_pass(bool const yesno);

	void	set_sock_addr(struct sockaddr_in *sock_addr);
	void	set_sock_len();


	// Member functions
	void disconnect(void);
	void append_message(std::string const &message);
	void clear_messages(void);
	void set_mode(UserMode const mode);
	void clear_mode(UserMode const mode);

	bool has_mode(UserMode const mode) const;

	ssize_t send_message(std::string const &message) const;
	ssize_t send_messages(void) const;

	std::string user_mask(void) const;
};

#endif
