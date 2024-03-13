/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:37:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/13 12:17:11 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/Password.hpp"

#define MAXIMUM_LENGTH_FOR_PASSWORD 50

inline static bool is_nolfcrspcl(char c)
{
	return (c >= 0x01 && c <= 0x09) || (c >= 0x0B && c <= 0x0C) || (c >= 0x0E && c <= 0x1F) || (c >= 0x21 && c <= 0x39)
	    || (c >= 0x3B);
}

inline static bool is_nolfcrsp(char c)
{
	return (c >= 0x01 && c <= 0x09) || (c >= 0x0B && c <= 0x0C) || (c >= 0x0E && c <= 0x1F) || (c >= 0x21);
}

inline static bool is_nolfcr(char c) { return c != '\n' && c != '\r'; }

/**
 * @brief Checks whether the password is valid.
 *
 * @return `true` if the password is valid, `false` otherwise.
 */
bool Password::is_valid() const
{
	if (this->empty() || this->length() > MAXIMUM_LENGTH_FOR_PASSWORD)
		return false;

	size_t const length = this->length();

	char const first_char = this->at(0);
	if (!is_nolfcrspcl(first_char))
		return false;

	for (size_t i = 1; i < length; ++i)
	{
		char c = this->at(i);
		if (c == ':')
		{
			for (size_t j = i + 1; j < length; ++j)
			{
				if (!is_nolfcr(this->at(j)))
					return false;
			}
			break; // Exit the loop after processing the portion following ':'
		}
		else if (!is_nolfcrsp(c))
			return false;
	}

	return true;
}
