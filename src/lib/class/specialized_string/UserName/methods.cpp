/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:42:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 10:12:00 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserName.hpp"

/**
 * @brief Check whether the username is valid.
 *
 * @return `true` if the username is valid, `false` otherwise.
 */
bool UserName::is_valid(void) const
{
	if (this->empty())
		return false;

	const char forbiddenChars[] = {0x00, 0x0A, 0x0D, 0x20, 0x40};
	for (size_t i = 0; i < 5; ++i)
	{
		if (this->find(forbiddenChars[i]) != std::string::npos)
			return false;
	}
	return true;
}
