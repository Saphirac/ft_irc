/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:06:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 14:58:31 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/ChannelName.hpp"

#define MAXIMUM_LENGTH_FOR_CHANNEL_NAME    50
#define LENGTH_FOR_CHANNEL_ID              5
#define EXCLUDED_CHARACTERS_FOR_CHANSTRING "\a\n\r ,:"
/**
 * @brief Check whether the channel name is valid.
 *
 * @return `true` if the channel name is valid, `false` otherwise.
 */
bool ChannelName::is_valid(void) const
{
	if (this->size() > MAXIMUM_LENGTH_FOR_CHANNEL_NAME)
		return false;

	char const prefix = *this->begin();
	size_t     name_begin = 1;

	if (std::string("#&+").find(prefix) == std::string::npos)
	{
		if (prefix != '!')
			return false;

		std::string const id = this->substr(1, LENGTH_FOR_CHANNEL_ID);

		if (id.size() < LENGTH_FOR_CHANNEL_ID || id.find_first_not_of(UPPERCASES DIGITS) != std::string::npos)
			return false;

		name_begin += LENGTH_FOR_CHANNEL_ID;
	}

	size_t const name_end = this->find_first_of(EXCLUDED_CHARACTERS_FOR_CHANSTRING, name_begin);

	if (name_end != std::string::npos)
	{
		if (this->at(name_end) != ':')
			return false;

		if (this->find_first_of(EXCLUDED_CHARACTERS_FOR_CHANSTRING, name_end + 1) != std::string::npos)
			return false;
	}

	return true;
}

/**
 * @brief Check whether the channel name is mode supported.
 *
 * @return `true` if the channel name is mode supported, `false` otherwise.
 */
bool ChannelName::are_modes_supported(void) const { return *this->begin() != '+'; }
