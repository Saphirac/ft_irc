/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithTime.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:25:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:30:08 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithTime : public std::exception
{
public:
	// Constructors
	ProblemWithTime(void) throw();

	// Destructor
	virtual ~ProblemWithTime(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
