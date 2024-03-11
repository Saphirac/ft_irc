/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:37:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 15:06:41 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/Password.hpp"

static bool is_nolfcr(char ch)
{
	return ch != '\n' && ch != '\r'
	    && (ch >= 0x01 && ch <= 0x09 || ch >= 0x0B && ch <= 0x0C || ch >= 0x0E && ch <= 0xFF);
}

static bool is_nolfcrsp(char ch) { return is_nolfcr(ch) && ch != ' '; }

static bool is_nolfcrspcl(char ch) { return is_nolfcrsp(ch) && !(ch >= 0x3A && ch <= 0x40); }

static bool is_nolfcrspat(char ch) { return is_nolfcrsp(ch) && ch != '@'; }

/**
 * @brief Checks whether the password is valid.
 *
 * @return `true` if the password is valid, `false` otherwise.
 */
bool Password::is_valid() const
{
	if (this->empty())
		return false;

	for (size_t index = this->begin(); index != this->end(); ++index)
	{
		char ch = *index;
		if (!is_nolfcr(ch) && !is_nolfcrsp(ch) && !is_nolfcrspcl(ch) && !is_nolfcrspat(ch))
		{
			return false;
		}
	}
	return true;
}