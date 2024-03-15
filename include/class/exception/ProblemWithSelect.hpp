/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSelect.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 03:23:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 15:20:07 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSelect : public std::exception
{
public:
	// Constructors
	ProblemWithSelect(void) throw();

	// Destructor
	virtual ~ProblemWithSelect(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
