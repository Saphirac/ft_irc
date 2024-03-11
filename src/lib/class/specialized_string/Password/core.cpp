/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:36:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 13:15:32 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Password.hpp"

// Constructors //

/**
 * @param password The nickname to initialize the instance with.
 */
Password::Password(std::string const &password) : std::string(password) {}

// Destructor //

Password::~Password(void) {}
