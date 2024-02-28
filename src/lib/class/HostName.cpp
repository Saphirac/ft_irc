/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:06:37 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:32:18 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/HostName.hpp"
#include "abnf_components.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param hostname The hostname to initialize the instance with.
 */
HostName::HostName(std::string const &hostname) : std::string(hostname) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
HostName::~HostName(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
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
