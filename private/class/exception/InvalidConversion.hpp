/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InvalidConversion.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:54:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:29:11 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class InvalidConversion : public std::exception
{
public:
	// Constructors
	InvalidConversion(void) throw();

	// Destructor
	virtual ~InvalidConversion(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
