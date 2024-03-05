/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithStrftime.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:24:51 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:31:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithStrftime : public std::exception
{
public:
	// Constructors
	ProblemWithStrftime(void) throw();

	// Destructor
	virtual ~ProblemWithStrftime(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
