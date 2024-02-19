/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Realname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:53:01 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 15:57:31 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Realname.hpp"

// ****************************************************************************************************************** //
//                                                    Constructors                                                    //
// ****************************************************************************************************************** //
/**
 * @brief Constructs a new Realname instance, initializing it with a given argument.
 *
 * @param realname The realname to initialize instance with.
 *
 * @return The newly created Realname instance.
 */
Realname::Realname(std::string const &realname) : std::string(realname) {}

/**
 * @brief Constructs a new Realname instance, copying a given source Realname instance.
 *
 * @param src The source Realname instance to copy.
 *
 * @return The newly created Realname instance.
 */
Realname::Realname(Realname const &src) : std::string(src) {}

// ****************************************************************************************************************** //
//                                                     Destructor                                                     //
// ****************************************************************************************************************** //
Realname::~Realname(void) {}
