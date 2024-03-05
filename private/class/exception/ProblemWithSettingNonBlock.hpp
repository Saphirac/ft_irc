/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProblemWithSettingNonBlock.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:24:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:34:59 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSettingNonBlock : public std::exception
{
public:
	// Constructors
	ProblemWithSettingNonBlock(void) throw();

	// Destructor
	virtual ~ProblemWithSettingNonBlock(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
