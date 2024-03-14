/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:37:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/14 16:58:36 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/Password.hpp"

#define MAXIMUM_LENGTH_FOR_PASSWORD 50

inline static bool is_nolfcrspcl(char const c)
{
	return std::string("\x00\r\n :").find(c) == std::string::npos;
}

inline static bool is_nolfcrsp(char const c)
{
	return std::string("\x00\r\n ").find(c) == std::string::npos;
}

inline static bool is_nolfcr(char const c)
{
	return std::string("\x00\r\n").find(c) == std::string::npos;
}

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
	
	if (!is_nolfcrspcl(this->at(0)))
		return false;

	for (size_t i = 1; i < length; ++i)
	{
		char const c = this->at(i);
		
		if (c == ':')
		{
			for (size_t j = i + 1; j < length; ++j)
			{
				if (!is_nolfcr(this->at(j)))
					return false;
			}
			return true;
		}
		if (!is_nolfcrsp(c))
			return false;
	}
	return true;
}
