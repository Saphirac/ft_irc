/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:54:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:54:55 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithBind.hpp"

// Constructors //

ProblemWithBind::ProblemWithBind(void) throw() : std::exception() {}

// Destructor //

ProblemWithBind::~ProblemWithBind(void) throw() {}
