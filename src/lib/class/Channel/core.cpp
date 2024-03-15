/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:43:58 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 15:00:58 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Channel.hpp"

// Constructors //

Channel::Modes::Flags::Flags(void) : _bits(0) {}

Channel::Modes::Modes(void) : _flags(), _limit(0), _key(), _operators(), _invite_masks(), _ban_masks() {}

Channel::Channel(bool const are_modes_supported) :
	_topic(),
	_are_modes_supported(are_modes_supported),
	_modes(),
	_members(),
	_invited_users()
{
}

// Destructor //

Channel::Modes::Flags::~Flags(void) {}

Channel::Modes::~Modes(void) {}

Channel::~Channel(void) {}

// Accessors //

Key const                      &Channel::Modes::get_key(void) const { return this->_key; }
size_t const                   &Channel::Modes::get_limit(void) const { return this->_limit; }
std::set<Client const *> const &Channel::Modes::get_operators(void) const { return this->_operators; }
std::set<NickName> const       &Channel::Modes::get_invite_masks(void) const { return this->_invite_masks; }
std::set<NickName> const       &Channel::Modes::get_ban_masks(void) const { return this->_ban_masks; }

Topic const          &Channel::get_topic(void) const { return this->_topic; }
bool                  Channel::get_are_modes_supported(void) const { return this->_are_modes_supported; }
Channel::Modes const &Channel::get_modes(void) const { return this->_modes; }

void Channel::set_topic(Topic const &topic) { this->_topic = topic; }
