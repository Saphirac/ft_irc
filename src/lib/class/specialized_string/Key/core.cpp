/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:51:48 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:52:24 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Key.hpp"

// Constructors //

/**
 * @param key The key to initialize the instance with.
 */
Key::Key(std::string const &key) : std::string(key) {}

// Destructor

Key::~Key(void) {}
