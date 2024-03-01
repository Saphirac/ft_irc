/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:15:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include "class/Channel.hpp"
#include "class/HostName.hpp"
#include "class/NickName.hpp"
#include "class/RealName.hpp"
#include "class/UserName.hpp"
#include <map>
#include <stdint.h>

class Channel;

class Client
{
public:
	// Shared fields
	static std::string const _default_away_msg;

	// Nested classes
	class Modes
	{
	public:
		// Constructors
		Modes(void);

		// Destructor
		~Modes(void);

		// Methods
		void set(UserMode const mode);
		void clear(UserMode const mode);
		bool is_set(UserMode const mode) const;

		std::string to_string(void) const;

	private:
		// Nested classes
		class Flags
		{
		public:
			// Constructors
			Flags(void);

			// Destructor
			~Flags(void);

			// Methods
			void set(UserMode const mode);
			void clear(UserMode const mode);
			bool is_set(UserMode const mode) const;

			std::string to_string(void) const;

		private:
			// Types
			typedef uint8_t _BitField;

			// Fields
			_BitField _bits;
		};

		// Fields
		Flags _flags;
	};

	// Constructors
	Client(
		int const       socket = -1,
		NickName const &nickname = NickName(),
		HostName const &hostname = HostName(),
		UserName const &username = UserName(),
		RealName const &realname = RealName());

	// Destructor
	~Client(void);

	// Accessors
	int                  get_socket(void) const;
	NickName const      &get_nickname(void) const;
	HostName const      &get_hostname(void) const;
	Client::Modes const &get_modes(void) const;
	std::string const   &get_away_msg(void) const;

	// Mutators
	void set_nickname(NickName const &nickname);
	void set_hostname(HostName const &hostname);
	void set_username(UserName const &username);
	void set_realname(RealName const &realname);
	void set_away_msg(std::string const &away_msg);

	// Methods
	void disconnect(void);

	void append_to_msg_in(std::string const &s);

	void append_to_msg_out(std::string const &msg);
	void send_msg_out(void);
	void clear_msg_out(void);

	void set_mode(UserMode const mode);
	void clear_mode(UserMode const mode);
	bool has_mode(UserMode const mode) const;

	std::string user_mask(void) const;

private:
	// Fields
	int         _socket;
	std::string _msg_in;
	std::string _msg_out;

	NickName      _nickname;
	HostName      _hostname;
	UserName      _username;
	RealName      _realname;
	Client::Modes _modes;

	std::string _away_msg;

	std::map<ChannelName, Channel *const> _joined_channels_by_name;
};
