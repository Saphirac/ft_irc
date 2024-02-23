/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:06:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 16:04:14 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/UserModes.hpp"
#include "user_modes.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param modes The modes to initialize the inner field with.
 */
UserModes::UserModes(_inner_type const modes) : _inner(modes) {}

/**
 * @param src The source UserModes instance to copy.
 */
UserModes::UserModes(UserModes const &src) : _inner(src._inner) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
UserModes::~UserModes(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Clears a given mode.
 *
 * @param mode The mode to clear.
 */
void UserModes::clear(UserMode const mode) { this->_inner &= ~(1 << mode); }

/**
 * @brief Sets a given mode.
 *
 * @param mode The mode to set.
 */
void UserModes::set(UserMode const mode) { this->_inner |= 1 << mode; }

/**
 * @brief Checks whether a given mode is set.
 *
 * @param mode The mode to check.
 *
 * @return true if the mode is set, false otherwise.
 */
bool UserModes::is_set(UserMode const mode) const { return this->_inner & 1 << mode; }

/**
 * @brief Converts the inner field to a string.
 *
 * @return The string representation of the inner field.
 */
std::string UserModes::to_string(void) const
{
	std::string modes;

	for (uint8_t i = 0; i < EndOfUserModes; ++i)
		if (this->is_set(static_cast<UserMode>(i)))
			modes += user_modes[i];

	return modes;
}
