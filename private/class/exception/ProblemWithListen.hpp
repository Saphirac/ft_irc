/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithListen.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:22:58 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:34:27 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithListen : public std::exception
{
public:
	// Constructors
	ProblemWithListen(void) throw();

	// Destructor
	virtual ~ProblemWithListen(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
