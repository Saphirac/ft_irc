/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnknownReply.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:25:32 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:30:01 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class UnknownReply : public std::exception
{
public:
	// Constructors
	UnknownReply(void) throw();

	// Destructor
	virtual ~UnknownReply(void) throw();

	// Methods
	virtual char const *what(void) const throw();
};
