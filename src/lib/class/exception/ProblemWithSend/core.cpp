/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:56:31 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:56:39 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithSend.hpp"

// Constructors //

ProblemWithSend::ProblemWithSend(void) throw() : std::exception() {}

// Destructor //

ProblemWithSend::~ProblemWithSend(void) throw() {}
