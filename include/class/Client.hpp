/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 05:46:54 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include "class/Channel.hpp"
#include "class/specialized_string/HostName.hpp"
#include "class/specialized_string/NickName.hpp"
#include "class/specialized_string/RealName.hpp"
#include "class/specialized_string/UserName.hpp"
#include <ctime>
#include <map>
#include <stdint.h>

class Channel;
class ChannelName;

class Client
{
public:
	// Shared fields
	static std::string const _default_quit_msg;

	// Nested classes
	class Modes
	{
	public:
		// Constructors
		Modes(void);

		// Destructor
		~Modes(void);

		// Accessors
		std::string const &get_away_msg(void) const;

		// Methods
		void set(UserMode const mode, void const *const arg = NULL);
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
		Flags       _flags;
		std::string _away_msg;
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
	bool                 get_has_been_pinged(void) const;
	std::string const   &get_ping_token(void) const;
	NickName const      &get_nickname(void) const;
	HostName const      &get_hostname(void) const;
	Client::Modes const &get_modes(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_last_msg_time(clock_t const time);
	void set_has_been_pinged(bool const has_been_pinged);
	void set_ping_token(std::string const &ping_token);
	void set_nickname(NickName const &nickname);
	void set_hostname(HostName const &hostname);
	void set_username(UserName const &username);
	void set_realname(RealName const &realname);

	// Methods
	void        append_to_msg_in(std::string const &s);
	std::string get_next_msg(void);
	clock_t     time_since_last_msg(void) const;

	std::string prefix(void) const;
	void        append_formatted_reply_to_msg_out(int const reply_number...);
	void        append_to_msg_out(std::string const &msg);
	void        send_msg_out(void);

	void set_mode(UserMode const mode, void const *const arg = NULL);
	void clear_mode(UserMode const mode);
	bool has_mode(UserMode const mode) const;

	std::string user_mask(void) const;

	void disconnect(void);

private:
	// Fields
	int         _socket;
	std::string _msg_in;
	std::string _msg_out;
	clock_t     _last_msg_time;
	bool        _has_been_pinged;
	std::string _ping_token;

	NickName      _nickname;
	HostName      _hostname;
	UserName      _username;
	RealName      _realname;
	Client::Modes _modes;

	std::map<ChannelName, Channel *const> _joined_channels_by_name;
};
