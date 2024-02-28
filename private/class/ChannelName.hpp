/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:05:02 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:30:48 by jodufour         ###   ########.fr       */
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
};
