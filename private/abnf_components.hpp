/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf_components.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 20:17:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 01:13:52 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

static std::string const uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::string const lowercase = "abcdefghijklmnopqrstuvwxyz";
static std::string const letter = uppercase + lowercase;
static std::string const digit = "0123456789";
static std::string const special = "[\\]^_`{|}";
static std::string const lfcrspcl = "\n\r :";
static std::string const lfcrsp = "\n\r ";
