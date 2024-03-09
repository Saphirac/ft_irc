/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:57:27 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:57:36 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/UnknownReply.hpp"

// Constructors //

UnknownReply::UnknownReply(void) throw() : std::exception() {}

// Destructor //

UnknownReply::~UnknownReply(void) throw() {}
