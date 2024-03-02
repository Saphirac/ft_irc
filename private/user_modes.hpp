/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_modes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:36:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:29:05 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include <cstddef>

static char const user_modes[] = {
	[Bot] = 'B',
	[LocalOperator] = 'O',
	[Away] = 'a',
	[Invisible] = 'i',
	[WallopsListener] = 'w',
};
static size_t const user_modes_len = sizeof(user_modes) / sizeof(*user_modes);
