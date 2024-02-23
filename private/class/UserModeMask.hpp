/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModeMask.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 23:57:01 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 01:15:57 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <string>

class UserModeMask : public std::string
{
public:
	// Constructors
	UserModeMask(std::string const &umode_mask = "");
	UserModeMask(UserModeMask const &src);

	// Destructor
	~UserModeMask(void);

	// Methods
	bool is_valid(uint8_t const umodes) const;
};
