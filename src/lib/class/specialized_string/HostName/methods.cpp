/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:34:17 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:35:48 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/HostName.hpp"

/**
 * @brief Check whether the hostname is valid.
 *
 * @return `true` if the hostname is valid, `false` otherwise.
 */
bool HostName::is_valid(void) const
{
	size_t begin = 0;
	size_t end;

	do
	{
		std::string const shortname = this->substr(begin, end = this->find('.'));

		if ((letter + digit).find(shortname[0]) == std::string::npos
		    || shortname.find_first_not_of(letter + digit + '-', 1) != std::string::npos)
			return false;

		begin += shortname.size() + 1;
	}
	while (end != std::string::npos);

	return true;
}
