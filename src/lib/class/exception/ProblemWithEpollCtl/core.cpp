/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:55:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:55:30 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithEpollCtl.hpp"

// Constructors //

ProblemWithEpollCtl::ProblemWithEpollCtl(void) throw() : std::exception() {}

// Destructor //

ProblemWithEpollCtl::~ProblemWithEpollCtl(void) throw() {}
