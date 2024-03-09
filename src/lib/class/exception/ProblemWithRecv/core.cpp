/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:56:03 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:56:24 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithRecv.hpp"

// Constructors //

ProblemWithRecv::ProblemWithRecv(void) throw() : std::exception() {}

// Destructor //

ProblemWithRecv::~ProblemWithRecv(void) throw() {}
