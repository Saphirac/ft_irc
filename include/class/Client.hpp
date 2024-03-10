/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 22:56:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/10 03:57:41 by mcourtoi         ###   ########.fr       */
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

#define DEFAULT_QUIT_MSG "Goodbye!"

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
	NickName const      &get_nickname(void) const;
	HostName const      &get_hostname(void) const;
	Client::Modes const &get_modes(void) const;
	std::map<ChannelName, Channel *const> const &get_joined_channels_by_name(void) const;

	// Mutators
	void set_socket(int const socket);
	void set_last_msg_time(std::clock_t const time);
	void set_nickname(NickName const &nickname);
	void set_hostname(HostName const &hostname);
	void set_username(UserName const &username);
	void set_realname(RealName const &realname);

	// Methods
	void         append_to_msg_in(std::string const &s);
	std::string  get_next_msg(void);
	std::clock_t time_since_last_msg(void) const;

	std::string prefix(void) const;
	void        append_formatted_reply_to_msg_out(int const reply_number...);
	void        append_to_msg_out(std::string const &msg);
	void        send_msg_out(void);

	void set_mode(UserMode const mode, void const *const arg = NULL);
	void clear_mode(UserMode const mode);
	bool has_mode(UserMode const mode) const;

	std::string user_mask(void) const;

	void join_channel(ChannelName const &chan_name, Channel &channel);
	void leave_channel(ChannelName const &chan_name);
	void disconnect(void);

private:
	// Fields
	int          _socket;
	std::string  _msg_in;
	std::string  _msg_out;
	std::clock_t _last_msg_time;

	NickName      _nickname;
	HostName      _hostname;
	UserName      _username;
	RealName      _realname;
	Client::Modes _modes;

	std::map<ChannelName, Channel *const> _joined_channels_by_name;
};
