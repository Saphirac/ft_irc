/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:48:13 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:53:04 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/NotAFlag.hpp"

// Constructors //

NotAFlag::NotAFlag(void) throw() : std::exception() {}

// Destructor //

NotAFlag::~NotAFlag(void) throw() {}
