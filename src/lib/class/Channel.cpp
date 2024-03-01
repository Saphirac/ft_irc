/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 01:29:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/01 08:30:28 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Channel.hpp"
#include "class/Exceptions.hpp"
#include <sstream>

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
Channel::Modes::Flags::Flags(void) : _bits(0) {}

Channel::Modes::Modes(void) : _flags(), _limit(0), _key(), _invite_masks(), _ban_masks(), _operators() {}

/**
 * @param name The name of the channel.
 */
Channel::Channel(void) : _topic(), _modes(), _members() {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Channel::Modes::Flags::~Flags(void) {}

Channel::Modes::~Modes(void) {}

Channel::~Channel(void) {}

// ***************************************************************************************************************** //
//                                                     Accessors                                                     //
// ***************************************************************************************************************** //
Key const                      &Channel::Modes::get_key(void) const { return this->_key; }
size_t const                   &Channel::Modes::get_limit(void) const { return this->_limit; }
std::set<NickName> const       &Channel::Modes::get_ban_masks(void) const { return this->_ban_masks; }
std::set<NickName> const       &Channel::Modes::get_invite_masks(void) const { return this->_invite_masks; }
std::set<Client const *> const &Channel::Modes::get_operators(void) const { return this->_operators; }

Topic const          &Channel::get_topic(void) const { return this->_topic; }
Channel::Modes const &Channel::get_modes(void) const { return this->_modes; }

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Sets a flag.
 *
 * @param flag The flag to set.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
void Channel::Modes::Flags::set(ChannelMode const flag)
{
	switch (flag)
	{
	case InviteOnly:
		this->_bits |= 1 << InviteOnly;
		return;
	case NoMessagesFromOutside:
		this->_bits |= 1 << NoMessagesFromOutside;
		return;
	case RestrictedTopic:
		this->_bits |= 1 << RestrictedTopic;
		return;
	default:
		throw NotAFlag();
	}
}

/**
 * @brief Clears a flag.
 *
 * @param flag The flag to clear.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
void Channel::Modes::Flags::clear(ChannelMode const flag)
{
	switch (flag)
	{
	case InviteOnly:
		this->_bits &= ~(1 << InviteOnly);
		return;
	case NoMessagesFromOutside:
		this->_bits &= ~(1 << NoMessagesFromOutside);
		return;
	case RestrictedTopic:
		this->_bits &= ~(1 << RestrictedTopic);
		return;
	default:
		throw NotAFlag();
	}
}

/**
 * @brief Checks whether a flag is set.
 *
 * @param flag The flag to check.
 *
 * @return `true` if the flag is set, `false` otherwise.
 *
 * @throw `NotAFlag` if the given `flag` isn't recognized.
 */
bool Channel::Modes::Flags::is_set(ChannelMode const flag) const
{
	switch (flag)
	{
	case InviteOnly:
		return (this->_bits & 1 << InviteOnly) != 0;
	case NoMessagesFromOutside:
		return (this->_bits & 1 << NoMessagesFromOutside) != 0;
	case RestrictedTopic:
		return (this->_bits & 1 << RestrictedTopic) != 0;
	default:
		throw NotAFlag();
	}
}

/**
 * @return The string representation of the flags that are currently set.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Channel::Modes::Flags::to_string(void) const
{
	std::string flags_as_string;

	if (this->is_set(InviteOnly))
		flags_as_string += CHANNEL_MODES[InviteOnly];
	if (this->is_set(NoMessagesFromOutside))
		flags_as_string += CHANNEL_MODES[NoMessagesFromOutside];
	if (this->is_set(RestrictedTopic))
		flags_as_string += CHANNEL_MODES[RestrictedTopic];

	return flags_as_string;
}

/**
 * @brief Sets a mode.
 *
 * @param mode The mode to set.
 * @param arg The argument associated with the mode if any.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Channel::Modes::set(ChannelMode const mode, void const *const arg)
{
	switch (mode)
	{
	case InviteOnly:
		this->_flags.set(InviteOnly);
		return;
	case NoMessagesFromOutside:
		this->_flags.set(NoMessagesFromOutside);
		return;
	case RestrictedTopic:
		this->_flags.set(RestrictedTopic);
		return;
	case Limit:
		this->_limit = *static_cast<size_t const *const>(arg);
		return;
	case KeyProtected:
		this->_key = *static_cast<Key const *const>(arg);
		return;
	case ChannelOperator:
		this->_operators.insert(static_cast<Client const *const>(arg));
		return;
	case InviteMask:
		this->_invite_masks.insert(*static_cast<NickName const *const>(arg));
		return;
	case BanMask:
		this->_ban_masks.insert(*static_cast<NickName const *const>(arg));
		return;
	}
}

/**
 * @brief Clears a mode.
 *
 * @param mode The mode to clear.
 * @param arg The argument associated with the mode if any.
 */
void Channel::Modes::clear(ChannelMode const mode, void const *const arg)
{
	switch (mode)
	{
	case InviteOnly:
		this->_flags.clear(InviteOnly);
		return;
	case NoMessagesFromOutside:
		this->_flags.clear(NoMessagesFromOutside);
		return;
	case RestrictedTopic:
		this->_flags.clear(RestrictedTopic);
		return;
	case Limit:
		this->_limit = 0;
		return;
	case KeyProtected:
		this->_key.clear();
		return;
	case ChannelOperator:
		this->_operators.erase(static_cast<Client const *const>(arg));
		return;
	case InviteMask:
		this->_invite_masks.erase(*static_cast<NickName const *const>(arg));
		return;
	case BanMask:
		this->_ban_masks.erase(*static_cast<NickName const *const>(arg));
		return;
	}
}

/**
 * @brief Checks whether a mode is set.
 *
 * @param mode The mode to check.
 *
 * @return `true` if the mode is set, `false` otherwise.
 */
bool Channel::Modes::is_set(ChannelMode const mode) const
{
	switch (mode)
	{
	case InviteOnly:
		return this->_flags.is_set(InviteOnly);
	case NoMessagesFromOutside:
		return this->_flags.is_set(NoMessagesFromOutside);
	case RestrictedTopic:
		return this->_flags.is_set(RestrictedTopic);
	case Limit:
		return this->_limit != 0;
	case KeyProtected:
		return !this->_key.empty();
	case ChannelOperator:
		return !this->_operators.empty();
	case InviteMask:
		return !this->_invite_masks.empty();
	case BanMask:
		return !this->_ban_masks.empty();
	}
}

/**
 * @brief Checks whether a nickname is marked as a ban mask.
 *
 * @param nickname The nickname to check.
 *
 * @return `true` if the nickname is marked as a ban mask, `false` otherwise.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
bool Channel::Modes::has_ban_mask(NickName const &nickname) const
{
	return this->_ban_masks.find(nickname) != this->_ban_masks.end();
}

/**
 * @brief Checks whether a nickname is marked as an invite mask.
 *
 * @param nickname The nickname to check.
 *
 * @return `true` if the nickname is marked as an invite mask, `false` otherwise.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
bool Channel::Modes::has_invite_mask(NickName const &nickname) const
{
	return this->_invite_masks.find(nickname) != this->_invite_masks.end();
}

/**
 * @brief Checks whether a client is marked as a channel operator.
 *
 * @param client The client to check.
 *
 * @return `true` if the client is marked as a channel operator, `false` otherwise.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
bool Channel::Modes::has_operator(Client const &client) const
{
	return this->_operators.find(&client) != this->_operators.end();
}

/**
 * @brief Generates the string representation of the modes that are currently set.
 *
 * @param include_operators whether the resulting string shall contain the ChannelOperator mode if it's set.
 * @param include_invite_masks whether the resulting string shall contain the InviteMask mode if it's set.
 * @param include_ban_masks whether the resulting string shall contain the BanMask mode if it's set.
 *
 * @return The string representation of the inner modes, whith their associated arguments if any.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
std::string Channel::Modes::to_string(
	bool const include_operators,
	bool const include_invite_masks,
	bool const include_ban_masks) const
{
	std::string modes_as_string = this->_flags.to_string();
	bool const  limit_is_set = this->is_set(Limit);
	bool const  key_is_set = this->is_set(KeyProtected);

	if (limit_is_set)
		modes_as_string += 'l';
	if (key_is_set)
		modes_as_string += 'k';
	if (include_operators)
		for (size_t i = 0; i < this->_operators.size(); ++i) modes_as_string += 'o';
	if (include_invite_masks)
		for (size_t i = 0; i < this->_invite_masks.size(); ++i) modes_as_string += 'I';
	if (include_ban_masks)
		for (size_t i = 0; i < this->_ban_masks.size(); ++i) modes_as_string += 'b';

	if (this->is_set(Limit))
	{
		std::stringstream ss;

		ss << this->_limit;
		modes_as_string += ' ' + ss.str();
	}
	if (key_is_set)
		modes_as_string += ' ' + this->_key;
	if (include_operators)
		for (std::set<Client const *>::const_iterator cit = this->_operators.begin(); cit != this->_operators.end();
		     ++cit)
			modes_as_string += ' ' + (*cit)->get_nickname();
	if (include_invite_masks)
		for (std::set<NickName>::const_iterator cit = this->_invite_masks.begin(); cit != this->_invite_masks.end();
		     ++cit)
			modes_as_string += ' ' + *cit;
	if (include_ban_masks)
		for (std::set<NickName>::const_iterator cit = this->_ban_masks.begin(); cit != this->_ban_masks.end(); ++cit)
			modes_as_string += ' ' + *cit;

	return modes_as_string;
}

/**
 * @brief Sets a mode for the channel.
 *
 * @param mode The mode to set.
 * @param arg The argument associated with the mode if any.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Channel::set_mode(ChannelMode const mode, void const *const arg) { this->_modes.set(mode, arg); }

/**
 * @brief Clears a mode for the channel.
 *
 * @param mode The mode to clear.
 * @param arg The argument associated with the mode if any.
 */
void Channel::clear_mode(ChannelMode const mode, void const *const arg) { this->_modes.clear(mode, arg); }

/**
 * @brief Adds a client to the list of the members of the channel.
 *
 * @param client The client to add.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Channel::add_member(Client &client) { this->_members.insert(&client); }

/**
 * @brief Removes a client from the list of the members of the channel.
    void broadcast_to_all_members_but_one(std::string const &msg, Client &client) const;
 *
 * @param client The client to remove.
 */
void Channel::remove_member(Client &client) { this->_members.erase(&client); }

/**
 * @brief Checks whether a client is a member of the channel.
 *
 * @param client The client to check.
 *
 * @return `true` if the client is a member of the channel, `false` otherwise.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
bool Channel::has_member(Client &client) const { return this->_members.find(&client) != this->_members.end(); }

/**
 * @brief Broadcasts a message to all the members of the channel.
 *
 * @param msg The message to broadcast.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Channel::broadcast_to_all_members(std::string const &msg) const
{
	for (std::set<Client *>::const_iterator cit = this->_members.begin(); cit != this->_members.end(); ++cit)
		(*cit)->append_to_msg_out(msg);
}
