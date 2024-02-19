/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_modes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:36:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 23:45:10 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_MODES_HPP
#define USER_MODES_HPP

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

#endif
