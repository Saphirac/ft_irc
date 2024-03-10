/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithFcntl.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:22:35 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:19:14 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithFcntl : public std::exception
{
public:
	// Constructors
	ProblemWithFcntl(void) throw();

	// Destructor
	virtual ~ProblemWithFcntl(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
