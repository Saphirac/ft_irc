/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:49:43 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/01 17:12:48 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/NickName.hpp"
#include "abnf_components.hpp"

#define MAXIMUM_LENGTH_FOR_NICKNAME 9

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param nickname The nickname to initialize the instance with.
 */
NickName::NickName(std::string const &nickname) : std::string(nickname) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
NickName::~NickName(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Checks whether the nickname is valid.
 *
 * @return `true` if the nickname is valid, `false` otherwise.
 */
bool NickName::is_valid(void) const
{
	return this->size() <= MAXIMUM_LENGTH_FOR_NICKNAME && (letter + special).find(this[0]) != this->npos
	    && this->find_first_not_of(letter + digit + special, 1) == this->npos;
}
