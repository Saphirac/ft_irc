/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithRecv.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:23:28 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:34:37 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithRecv : public std::exception
{
public:
	// Constructors
	ProblemWithRecv(void) throw();

	// Destructor
	virtual ~ProblemWithRecv(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
