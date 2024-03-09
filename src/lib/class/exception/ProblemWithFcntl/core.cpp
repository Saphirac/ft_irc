/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:55:43 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:19:40 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithFcntl.hpp"

// Constructors //

ProblemWithFcntl::ProblemWithFcntl(void) throw() : std::exception() {}

// Destructor //

ProblemWithFcntl::~ProblemWithFcntl(void) throw() {}
