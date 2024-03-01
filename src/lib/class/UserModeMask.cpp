/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModeMask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 00:01:29 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/01 17:14:41 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/UserModeMask.hpp"

#define MAXIMUM_LENGTH_FOR_USER_MODE_MASK 2

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param umode_mask The user mode mask to initialize the inner field with.
 */
UserModeMask::UserModeMask(std::string const &umode_mask) : std::string(umode_mask) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
UserModeMask::~UserModeMask(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Check whether the user mode mask is valid.
 *
 * @param umodes The user mode mask to check.
 *
 * @return `true` if the user mode mask is valid, `false` otherwise.
 */
bool UserModeMask::is_valid(uint8_t const umodes) const
{
	return this->size() <= MAXIMUM_LENGTH_FOR_USER_MODE_MASK && !(umodes & ~12);
}
