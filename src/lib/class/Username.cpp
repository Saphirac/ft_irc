/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:31:00 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 01:16:10 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Username.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Username instance, initializing its inner field with a given argument.
 *
 * @param username The username to initialize the inner field with.
 *
 * @return The newly created Username instance.
 */
Username::Username(std::string const &username) : std::string(username) {}

/**
 * @brief Constructs a new Username instance, copying a given source Username instance.
 *
 * @param src The source Username instance to copy.
 *
 * @return The newly created Username instance.
 */
Username::Username(Username const &src) : std::string(src) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Username::~Username(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Check whether the username is valid.
 *
 * @return true if the username is valid, false otherwise.
 */
bool Username::is_valid(void) const { return this->find('@') == std::string::npos; }
