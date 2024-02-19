/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModeMask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 00:01:29 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 23:07:29 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/UserModeMask.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new UserModeMask instance from a given argument.
 *
 * @param umode_mask The user mode mask to initialize the inner field with.
 *
 * @return The newly created UserModeMask instance.
 */
UserModeMask::UserModeMask(std::string const &umode_mask) : std::string(umode_mask) {}

/**
 * @brief Constructs a new UserModeMask instance, copying a given source UserModeMask instance.
 *
 * @param src The source UserModeMask instance to copy.
 *
 * @return The newly created UserModeMask instance.
 */
UserModeMask::UserModeMask(UserModeMask const &src) : std::string(src) {}

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
 * @return true if the user mode mask is valid, false otherwise.
 */
bool UserModeMask::is_valid(uint8_t const umodes) const { return this->size() < 3 && !(umodes & ~12); }
