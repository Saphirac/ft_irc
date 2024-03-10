/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithEpollWait.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:21:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:33:54 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithEpollWait : public std::exception
{
public:
	// Constructors
	ProblemWithEpollWait(void) throw();

	// Destructor
	virtual ~ProblemWithEpollWait(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
