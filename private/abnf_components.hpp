/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abnf_components.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 20:17:21 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 17:56:03 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#define UPPERCASES "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASES "abcdefghijklmnopqrstuvwxyz"
#define LETTERS    UPPERCASES LOWERCASES
#define DIGITS     "0123456789"
#define SPECIALS   "[\\]^_`{|}"
#define DASH       "-"
