/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSetSockOpt.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 04:59:00 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 05:00:25 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSetSockOpt : public std::exception
{
public:
	// Constructors
	ProblemWithSetSockOpt(void) throw();

	// Destructor
	virtual ~ProblemWithSetSockOpt(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
