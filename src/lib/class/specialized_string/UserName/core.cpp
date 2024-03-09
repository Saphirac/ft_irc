/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:41:41 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:42:13 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserName.hpp"

// Constructors //

/**
 * @param username The username to initialize the inner field with.
 */
UserName::UserName(std::string const &username) : std::string(username) {}

// Destructor //

UserName::~UserName(void) {}
