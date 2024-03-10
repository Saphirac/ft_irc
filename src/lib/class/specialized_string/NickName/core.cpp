/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:36:34 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:37:04 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/NickName.hpp"

// Constructors //

/**
 * @param nickname The nickname to initialize the instance with.
 */
NickName::NickName(std::string const &nickname) : std::string(nickname) {}

// Destructor //

NickName::~NickName(void) {}
