/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NotAFlag.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:55:01 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:31:56 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class NotAFlag : public std::exception
{
public:
	// Constructors
	NotAFlag(void) throw();

	// Destructor
	virtual ~NotAFlag(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
