/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:03:36 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 08:51:47 by mcourtoi         ###   ########.fr       */
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
	bool is_mode_supported(void) const;
	
};
