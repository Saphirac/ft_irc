/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserNotOnChannel.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 03:08:06 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 03:08:30 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class UserNotOnChannel : public std::exception
{
public:
	// Constructors
	UserNotOnChannel(void) throw();

	// Destructor
	virtual ~UserNotOnChannel(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
