/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:54:59 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:55:06 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithClock.hpp"

// Constructors //

ProblemWithClock::ProblemWithClock(void) throw() : std::exception() {}

// Destructor //

ProblemWithClock::~ProblemWithClock(void) throw() {}
