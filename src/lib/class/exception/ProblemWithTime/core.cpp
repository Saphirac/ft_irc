/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:57:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:57:24 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithTime.hpp"

// Constructors //

ProblemWithTime::ProblemWithTime(void) throw() : std::exception() {}

// Destructor //

ProblemWithTime::~ProblemWithTime(void) throw() {}
