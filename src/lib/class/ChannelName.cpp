/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:08:57 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:32:39 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/ChannelName.hpp"
#include "abnf_components.hpp"

#define EXCLUDED_CHARACTERS_FOR_CHANSTRING "\a\n\r ,:"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @param channel_name The channel name to initialize the instance with.
 */
ChannelName::ChannelName(std::string const &channel_name) : std::string(channel_name) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
ChannelName::~ChannelName(void) {}

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
/**
 * @brief Check whether the channel name is valid.
 *
 * @return `true` if the channel name is valid, `false` otherwise.
 */
bool ChannelName::is_valid(void) const
{
	if (this->size() > 50)
		return false;

	char const prefix = *this->begin();
	size_t     name_begin = 1;

	if (std::string("#&+").find(prefix) == std::string::npos)
	{
		if (prefix != '!')
			return false;

		std::string const id = this->substr(1, 5);

		if (id.size() < 5 || id.find_first_not_of(uppercase + digit) != std::string::npos)
			return false;

		name_begin = 6;
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
