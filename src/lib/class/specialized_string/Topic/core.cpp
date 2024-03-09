/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:49:55 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:50:31 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Topic.hpp"

// Constructors

/**
 * @param topic The topic to initialize the instance with.
 */
Topic::Topic(std::string const &topic) : std::string(topic) {}

// Destructor

Topic::~Topic(void) {}
