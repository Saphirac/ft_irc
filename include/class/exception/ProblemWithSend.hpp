/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSend.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:23:49 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:34:48 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSend : public std::exception
{
public:
	// Constructors
	ProblemWithSend(void) throw();

	// Destructor
	virtual ~ProblemWithSend(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
