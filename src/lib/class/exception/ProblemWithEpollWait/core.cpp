/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:55:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:55:41 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithEpollWait.hpp"

// Constructors //

ProblemWithEpollWait::ProblemWithEpollWait(void) throw() : std::exception() {}

// Destructor //

ProblemWithEpollWait::~ProblemWithEpollWait(void) throw() {}
