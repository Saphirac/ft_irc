/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_modes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:36:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/08 23:45:27 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include <map>

static std::pair<char, UserMode> const raw_user_modes[] = {
	std::make_pair(USER_MODES[Bot], Bot),
	std::make_pair(USER_MODES[LocalOperator], LocalOperator),
	std::make_pair(USER_MODES[Away], Away),
	std::make_pair(USER_MODES[Invisible], Invisible),
	std::make_pair(USER_MODES[WallopsListener], WallopsListener),
};
static std::map<char, UserMode> const user_modes(
	raw_user_modes,
	raw_user_modes + sizeof(raw_user_modes) / sizeof(*raw_user_modes));

typedef std::map<char, UserMode>::const_iterator UserModeIterator;
