/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:39:22 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:40:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserModeMask.hpp"

// Constructors //

/**
 * @param umode_mask The user mode mask to initialize the inner field with.
 */
UserModeMask::UserModeMask(std::string const &umode_mask) : std::string(umode_mask) {}

// Destructor //

UserModeMask::~UserModeMask(void) {}
