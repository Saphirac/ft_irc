/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:03:36 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 13:05:17 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ChannelName : public std::string
{
public:
	// Constructors
	ChannelName(std::string const &channel_name = "");

	// Destructor
	~ChannelName(void);

	// Methods
	bool is_valid(void) const;
	bool are_modes_supported(void) const;
};
