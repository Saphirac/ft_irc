/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:31:06 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 23:08:28 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class NotAFlag : public std::exception
{
public:
	// Methods
	virtual char const *what(void) const throw();
};

class ProblemWithFormatReply : public std::exception
{
public:
	// Methods
	virtual char const *what(void) const throw();
};

class ProblemWithSend : public std::exception
{
public:
	// Methods
	virtual char const *what(void) const throw();
};
