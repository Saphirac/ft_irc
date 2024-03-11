/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:23:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 13:05:17 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ChannelMode.hpp"
#include "class/Client.hpp"
#include "class/specialized_string/ChannelName.hpp"
#include "class/specialized_string/Key.hpp"
#include "class/specialized_string/Topic.hpp"
#include <map>
#include <set>
#include <stdint.h>

class Client;
class NickName;

class Channel
{
public:
	// Nested classes
	class Modes
	{
	public:
		// Constructors
		Modes(void);

		// Destructor
		~Modes(void);

		// Accessors
		Key const                      &get_key(void) const;
		size_t const                   &get_limit(void) const;
		std::set<Client const *> const &get_operators(void) const;
		std::set<NickName> const       &get_invite_masks(void) const;
		std::set<NickName> const       &get_ban_masks(void) const;

		// Methods
		void set(ChannelMode const mode, void const *const arg = NULL);
		void clear(ChannelMode const mode, void const *const arg = NULL);

		bool is_set(ChannelMode const mode) const;
		bool has_operator(Client const &client) const;
		bool has_invite_mask(NickName const &nickname) const;
		bool has_ban_mask(NickName const &nickname) const;

		std::string to_string(
			bool const include_operators = false,
			bool const include_invite_masks = false,
			bool const include_ban_mask = false) const;

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
			void set(ChannelMode const mode);
			void clear(ChannelMode const mode);
			bool is_set(ChannelMode const mode) const;

			std::string to_string(void) const;

		private:
			// Types
			typedef uint8_t _BitField;

			// Fields
			_BitField _bits;
		};

		// Types
		typedef std::set<Client const *> _OperatorSet;
		typedef std::set<NickName>       _MaskSet;

		// Fields
		Flags        _flags;
		size_t       _limit;
		Key          _key;
		_OperatorSet _operators;
		_MaskSet     _invite_masks;
		_MaskSet     _ban_masks;
	};

	// Constructors
	Channel(bool const are_modes_supported = true);

	// Destructor
	~Channel(void);

	// Accessors
	Topic const          &get_topic(void) const;
	bool                  get_are_modes_supported(void) const;
	Channel::Modes const &get_modes(void) const;

	// Setters

	void set_topic(Topic const &topic);

	// Methods
	void set_mode(ChannelMode const mode, void const *const arg = NULL);
	void clear_mode(ChannelMode const mode, void const *const arg = NULL);

	void        add_member(Client &user);
	void        remove_member(Client &user);
	bool        has_member(Client &user) const;
	size_t      member_count(void) const;
	std::string members_as_string(void) const;
	void        broadcast_to_all_members(std::string const &msg) const;

	void add_invited_user(Client const &user, bool const is_invited_by_operator = false);
	void remove_invited_user(Client const &user);
	bool has_invited_user(Client const &user) const;
	bool has_invited_user_by_operator(Client const &user) const;

private:
	// Types
	typedef std::set<Client *>              _MemberSet;
	typedef _MemberSet::const_iterator      _MemberIterator;
	typedef std::map<Client const *, bool>  _InvitedUserMap;
	typedef _InvitedUserMap::const_iterator _InvitedUserIterator;

	// Fields
	Topic           _topic;
	bool            _are_modes_supported;
	Modes           _modes;
	_MemberSet      _members;
	_InvitedUserMap _invited_users;
};
