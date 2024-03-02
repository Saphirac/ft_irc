/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf_components.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 20:17:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:11:25 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

static std::string const uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::string const lowercase = "abcdefghijklmnopqrstuvwxyz";
static std::string const letter = uppercase + lowercase;
static std::string const digit = "0123456789";
static std::string const special = "[\\]^_`{|}";
