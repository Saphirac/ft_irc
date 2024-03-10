/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithAccept.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:55:45 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:32:23 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithAccept : public std::exception
{
public:
	// Constructors
	ProblemWithAccept(void) throw();

	// Destructor
	virtual ~ProblemWithAccept(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
