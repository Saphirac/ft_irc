/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:48 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/10 06:39:12 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Prefix.hpp"

/**
 * Checks if the given character is alphanumeric (either a digit or a letter).
 *
 * @param c The character to check.
 * @return True if the character is alphanumeric, false otherwise.
 */
static bool is_alphanumeric(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
/**
 * Determines if a character is within the valid ranges specified by the
 * nolfcrspat rule.
 *
 * @param c The character to validate, passed as a signed char but treated as unsigned.
 * @return True if the character meets the nolfcrspat criteria, false otherwise.
 */
static bool is_valid_nolfcrspat(char c)
{
	unsigned char uc = c;

	if ((uc >= 0x01 && uc <= 0x09) || (uc >= 0x0B && uc <= 0x0C) || (uc >= 0x0E && uc <= 0x1F)
	    || (uc >= 0x21 && uc <= 0x3F) || (uc >= 0x41 && uc <= 0xFF))
	{
		return true;
	}

	return false;
}
/**
 * Validates if a given string qualifies as a valid shortname according to
 * specific criteria.
 *
 * @param name The string to validate as a shortname.
 * @return True if the string is a valid shortname, false otherwise.
 */
static bool is_valid_shortname(const std::string &name)
{
	if (name.empty() || !is_alphanumeric(name[0]))
		return false;
	for (size_t i = 0; i < name.length(); ++i)
	{
		if (!is_alphanumeric(name[i]) && name[i] != '-')
			return false;
	}
	return true;
}
/**
 * Validates whether a given string can serve as a valid server or host name.
 *
 * @param name The string to validate as a server or host name.
 * @return True if the string is a valid server or host name, false otherwise.
 */
static bool is_valid_server_or_host(const std::string &name)
{
	size_t start = 0;
	size_t end = name.find('.');
	while (end != std::string::npos)
	{
		if (!is_valid_shortname(name.substr(start, end - start)))
			return false;
		start = end + 1;
		end = name.find('.', start);
	}
	return is_valid_shortname(name.substr(start));
}
/**
 * Checks if a given string is a valid username.
 *
 * @param user The username string to validate.
 * @return True if the string is a valid username, false otherwise.
 */
static bool is_valid_user(const std::string &user)
{
	if (user.empty())
		return false;
	for (size_t i = 0; i < user.length(); ++i)
	{
		if (!is_valid_nolfcrspat(user[i]))
			return false;
	}
	return true;
}
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

	if ((*this)[0] != ':')
		return false;

	std::string rest = this->substr(1);
	size_t      bang_pos = rest.find('!');
	size_t      at_pos = rest.find('@');

	if (bang_pos == std::string::npos && at_pos == std::string::npos)
		return is_valid_server_or_host(rest);
	else if (bang_pos != std::string::npos && at_pos != std::string::npos && bang_pos < at_pos)
	{
		// Nick!User@Host
		std::string nick = rest.substr(0, bang_pos);
		std::string user = rest.substr(bang_pos + 1, at_pos - bang_pos - 1);
		std::string host = rest.substr(at_pos + 1);

		return is_valid_shortname(nick) && is_valid_user(user) && is_valid_server_or_host(host);
	}
	else
		return false;
}
