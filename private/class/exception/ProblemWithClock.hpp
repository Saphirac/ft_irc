/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithClock.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:19:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:33:19 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithClock : public std::exception
{
public:
	// Constructors
	ProblemWithClock(void) throw();

	// Destructor
	virtual ~ProblemWithClock(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
