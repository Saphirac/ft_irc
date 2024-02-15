/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf_components.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 20:17:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/07 08:31:17 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABNF_COMPONENTS_HPP
#define ABNF_COMPONENTS_HPP

#include <string>

static std::string const uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::string const lowercase = "abcdefghijklmnopqrstuvwxyz";
static std::string const letter = uppercase + lowercase;
static std::string const digit = "0123456789";
static std::string const special = "[\\]^_`{|}";

#endif