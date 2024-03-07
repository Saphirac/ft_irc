/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:51:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/06 20:05:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/Key.hpp"

#define MINIMUM_LENGTH_FOR_KEY      1
#define MAXIMUM_LENGTH_FOR_KEY      23
#define EXCLUDED_CHARACTERS_FOR_KEY "\t\n\v\f\r ,"
/**
 * @brief Checks whether the key is valid.
 *
 * @return `true` if the key is valid, `false` otherwise.
 */
bool Key::is_valid(void) const
{
	size_t const len = this->size();

	return len >= MINIMUM_LENGTH_FOR_KEY && len <= MAXIMUM_LENGTH_FOR_KEY
	    && this->find_first_of(EXCLUDED_CHARACTERS_FOR_KEY) == std::string::npos;
}
