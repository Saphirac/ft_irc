/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithEpollCtl.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:20:49 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:33:43 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithEpollCtl : public std::exception
{
public:
	// Constructors
	ProblemWithEpollCtl(void) throw();

	// Destructor
	virtual ~ProblemWithEpollCtl(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
