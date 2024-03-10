/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithBind.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:17:19 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:32:56 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithBind : public std::exception
{
public:
	// Constructors
	ProblemWithBind(void) throw();

	// Destructor
	virtual ~ProblemWithBind(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
