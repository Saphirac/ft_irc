/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hostname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:06:37 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 16:03:08 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Hostname.hpp"
#include "abnf_components.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param hostname The hostname to initialize the inner field with.
 */
Hostname::Hostname(std::string const &hostname) : std::string(hostname) {}

/**
 * @param src The source Hostname instance to copy.
 */
Hostname::Hostname(Hostname const &src) : std::string(src) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Hostname::~Hostname(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Check whether the hostname is valid.
 *
 * @return true if the hostname is valid, false otherwise.
 */
bool Hostname::is_valid(void) const
{
	size_t pos = 0;

	do
	{
		std::string const shortname = this->substr(pos, this->find('.'));

		if ((letter + digit).find(shortname[0]) == std::string::npos
		    || shortname.find_first_not_of(letter + digit + "-", 1) != std::string::npos)
			return false;

		pos += shortname.size() + 1;
	}
	while (this->find('.') != std::string::npos);

	return true;
}
