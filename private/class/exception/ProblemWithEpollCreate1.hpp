/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWith.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:20:08 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:32:12 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithEpollCreate1 : public std::exception
{
public:
	// Constructors
	ProblemWithEpollCreate1(void) throw();

	// Destructor
	virtual ~ProblemWithEpollCreate1(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
