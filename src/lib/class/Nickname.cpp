/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:49:43 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 16:03:31 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Nickname.hpp"
#include "abnf_components.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param nickname The nickname to initialize the instance with.
 */
Nickname::Nickname(std::string const &nickname) : std::string(nickname) {}

/**
 * @param src The source Nickname instance to copy.
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
