/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithConnect.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 03:23:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 03:26:38 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithConnect : public std::exception
{
public:
	// Constructors
	ProblemWithConnect(void) throw();

	// Destructor
	virtual ~ProblemWithConnect(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
