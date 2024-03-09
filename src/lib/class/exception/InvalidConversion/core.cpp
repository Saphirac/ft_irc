/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:45:39 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:51:31 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/InvalidConversion.hpp"

// Constructors //

InvalidConversion::InvalidConversion(void) throw() : std::exception() {}

// Destructor //

InvalidConversion::~InvalidConversion(void) throw() {}
