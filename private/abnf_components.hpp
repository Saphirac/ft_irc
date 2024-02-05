/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf_components.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 20:17:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/05 20:33:53 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABNF_COMPONENTS_HPP
#define ABNF_COMPONENTS_HPP

#include <string>

static std::string const UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::string const LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
static std::string const LETTER = UPPERCASE + LOWERCASE;
static std::string const DIGIT = "0123456789";
static std::string const SPECIAL = "[\\]^_`{|}";

#endif