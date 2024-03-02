/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModeMask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 00:01:29 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 16:04:02 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/UserModeMask.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param umode_mask The user mode mask to initialize the inner field with.
 */
UserModeMask::UserModeMask(std::string const &umode_mask) : std::string(umode_mask) {}

/**
 * @param src The source UserModeMask instance to copy.
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
