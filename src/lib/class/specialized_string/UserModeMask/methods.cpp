/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:40:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:41:02 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserModeMask.hpp"

#define MAXIMUM_LENGTH_FOR_USER_MODE_MASK 2
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
