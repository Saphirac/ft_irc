/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:36 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/14 18:50:52 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Prefix.hpp"

/**
 * @param prefix The prefix to initialize the instance with.
 */
Prefix::Prefix(std::string const &prefix) : std::string(prefix) {}

// Destructor //

Prefix::~Prefix(void) {}
