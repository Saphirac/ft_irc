/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:37:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/06 02:46:51 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/NickName.hpp"

#define MAXIMUM_LENGTH_FOR_NICKNAME 9
/**
 * @brief Checks whether the nickname is valid.
 *
 * @return `true` if the nickname is valid, `false` otherwise.
 */
bool NickName::is_valid(void) const
{
	return this->size() <= MAXIMUM_LENGTH_FOR_NICKNAME && (letter + special).find(this->at(0)) != this->npos
	    && this->find_first_not_of(letter + digit + special, 1) == this->npos;
}
