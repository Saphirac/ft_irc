/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:33:27 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:33:59 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/HostName.hpp"

// Constructors //

/**
 * @param hostname The hostname to initialize the instance with.
 */
HostName::HostName(std::string const &hostname) : std::string(hostname) {}

// Destructor //

HostName::~HostName(void) {}
