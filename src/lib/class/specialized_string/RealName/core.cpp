/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:38:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:38:50 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/RealName.hpp"

// Constructors //

/**
 * @param realname The realname to initialize the instance with.
 */
RealName::RealName(std::string const &realname) : std::string(realname) {}

// Destructor //

RealName::~RealName(void) {}
