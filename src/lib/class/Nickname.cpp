/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:49:43 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 15:56:35 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Nickname.hpp"
#include "abnf_components.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Nickname instance, initializing it with a given argument.
 *
 * @param nickname The nickname to initialize the instance with.
 *
 * @return The newly created Nickname instance.
 */
Nickname::Nickname(std::string const &nickname) : std::string(nickname) {}

/**
 * @brief Constructs a new Nickname instance, copying a given source Nickname instance.
 *
 * @param src The source Nickname instance to copy.
 *
 * @return The newly created Nickname instance.
 */
Nickname::Nickname(Nickname const &src) : std::string(src) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Nickname::~Nickname(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Checks whether the nickname is valid.
 *
 * @return true if the nickname is valid, false otherwise.
 */
bool Nickname::is_valid(void) const
{
	return this->size() < 10 && (letter + special).find(this[0]) != this->npos
	    && this->find_first_not_of(letter + digit + special, 1) == this->npos;
}
