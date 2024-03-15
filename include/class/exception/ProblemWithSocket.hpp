/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSocket.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:24:32 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:35:08 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSocket : public std::exception
{
public:
	// Constructors
	ProblemWithSocket(void) throw();

	// Destructor
	virtual ~ProblemWithSocket(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
