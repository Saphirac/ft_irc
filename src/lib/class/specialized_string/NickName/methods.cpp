/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:37:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 18:01:28 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/NickName.hpp"

#define MAXIMUM_LENGTH_FOR_NICKNAME 9

static std::string const letters_specials = LETTERS SPECIALS;

/**
 * @brief Checks whether the nickname is valid.
 *
 * @return `true` if the nickname is valid, `false` otherwise.
 */
bool NickName::is_valid(void) const
{
	return !this->empty() && this->size() <= MAXIMUM_LENGTH_FOR_NICKNAME
	    && letters_specials.find(this->at(0)) != this->npos
	    && this->find_first_not_of(LETTERS DIGITS SPECIALS DASH, 1) == this->npos;
}
