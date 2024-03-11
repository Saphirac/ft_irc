/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:48 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/11 03:43:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/HostName.hpp"
#include "class/specialized_string/NickName.hpp"
#include "class/specialized_string/Prefix.hpp"
#include "class/specialized_string/UserName.hpp"
#include <cctype>

/**
 * Determines if the current Prefix instance represents a valid prefix according
 * to the IRC protocol's specifications.
 *
 * @return True if the Prefix instance is valid, false if it is either empty, does not
 * start with a colon, or contains an invalid server, nickname, user, or host structure.
 */
bool Prefix::is_valid(void) const
{
	if (this->empty())
		return true;

	if (this->at(0) != ':')
		return false;

	size_t const bang_pos = this->find('!', 1);
	size_t const at_pos = this->find('@', 1);

	// TODO: Determine whether the first part is a server name or a nickname
	if (/* The first part is a server name */)
	{
		// TODO: Check that the server name is valid
		return true;
	}
	// TODO: Get the provided nickname and check that it is valid
	if (bang_pos != std::string::npos)
	{
		// TODO: Get the provided user name and check that it is valid
		// TODO: Get the provided host name and check that it is valid
		return true;
	}
	if (at_pos != std::string::npos)
	{
		// TODO: Get the provided host name and check that it is valid
	}
	return true;
}
