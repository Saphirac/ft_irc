/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:48 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/13 15:32:23 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/HostName.hpp"
#include "class/specialized_string/NickName.hpp"
#include "class/specialized_string/Prefix.hpp"
#include "class/specialized_string/UserName.hpp"

/**
 * Determines if the current Prefix instance represents a valid prefix according
 * to the IRC protocol's specifications.
 *
 * @return True if the Prefix instance is valid, false if it is either empty, does not
 * start with a colon, or contains an invalid server, nickname, user, or host structure.
 */
bool Prefix::is_valid(void)
{
	if (this->empty())
		return true;

	if (this->at(0) != ':')
		return false;

	size_t const      end_server_or_nick = this->find_first_of("!@", 1);
	std::string server_or_nick = this->substr(1, end_server_or_nick - 1);

	if (end_server_or_nick == std::string::npos)
		return HostName(server_or_nick).is_valid();

	if (!NickName(server_or_nick).is_valid())
		return false;
	
	this->_sender = server_or_nick;

	std::string const remaining = this->substr(end_server_or_nick + 1);

	if (this->at(end_server_or_nick) == '!')
	{
		size_t at_pos = remaining.find('@');

		if (at_pos == std::string::npos)
			return false;

		return UserName(remaining.substr(0, at_pos)).is_valid() && HostName(remaining.substr(at_pos + 1)).is_valid();
	}
	return HostName(remaining).is_valid();
}
