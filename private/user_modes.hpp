/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_modes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 01:14:35 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/23 02:27:10 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include <cstddef>
#include <map>

static std::pair<char, UserMode> const raw_user_modes[] = {
	std::make_pair(USER_MODES[Bot], Bot),
	std::make_pair(USER_MODES[LocalOperator], LocalOperator),
	std::make_pair(USER_MODES[Away], Away),
	std::make_pair(USER_MODES[Invisible], Invisible),
	std::make_pair(USER_MODES[WallopsListener], WallopsListener),
};
static size_t const raw_user_modes_len = sizeof(raw_user_modes) / sizeof(*raw_user_modes);

static std::map<char, UserMode> const user_modes(raw_user_modes, raw_user_modes + raw_user_modes_len);

typedef std::map<char, UserMode>::const_iterator UserModeIterator;
