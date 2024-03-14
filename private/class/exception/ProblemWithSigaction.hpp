/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSigaction.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 03:23:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 03:23:29 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSigaction : public std::exception
{
public:
	// Constructors
	ProblemWithSigaction(void) throw();

	// Destructor
	virtual ~ProblemWithSigaction(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
