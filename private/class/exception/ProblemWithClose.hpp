/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithClose.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:19:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:33:32 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithClose : public std::exception
{
public:
	// Constructors
	ProblemWithClose(void) throw();

	// Destructor
	virtual ~ProblemWithClose(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
