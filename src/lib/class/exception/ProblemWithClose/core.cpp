/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:55:09 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:55:17 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithClose.hpp"

// Constructors //

ProblemWithClose::ProblemWithClose(void) throw() : std::exception() {}

// Destructor //

ProblemWithClose::~ProblemWithClose(void) throw() {}
