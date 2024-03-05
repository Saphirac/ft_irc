/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:56:54 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:57:00 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithSocket.hpp"

// Constructors //

ProblemWithSocket::ProblemWithSocket(void) throw() : std::exception() {}

// Destructor //

ProblemWithSocket::~ProblemWithSocket(void) throw() {}
