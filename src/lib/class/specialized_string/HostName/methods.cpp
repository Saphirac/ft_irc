/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:34:17 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 18:00:06 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/HostName.hpp"

static std::string const letters_digits = LETTERS DIGITS;

/**
 * @brief Check whether the host name is valid.
 *
 * @return `true` if the host name is valid, `false` otherwise.
 */
bool HostName::is_valid(void) const
{
	size_t begin = 0;
	size_t end;

	do
	{
		std::string const shortname = this->substr(begin, (end = this->find('.', begin)) - begin);

		if (letters_digits.find(shortname[0]) == std::string::npos
		    || shortname.find_first_not_of(LETTERS DIGITS DASH, 1) != std::string::npos)
			return false;

		begin += shortname.size() + 1;
	}
	while (end != std::string::npos);

	return true;
}
