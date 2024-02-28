/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 22:57:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 01:35:33 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ChannelMode.hpp"
#include <cstddef>
#include <map>

static std::pair<char, ChannelMode> const raw_channel_modes[] = {
	std::make_pair(CHANNEL_MODES[InviteOnly], InviteOnly),
	std::make_pair(CHANNEL_MODES[NoMessagesFromOutside], NoMessagesFromOutside),
	std::make_pair(CHANNEL_MODES[RestrictedTopic], RestrictedTopic),
	std::make_pair(CHANNEL_MODES[InviteMask], InviteMask),
	std::make_pair(CHANNEL_MODES[BanMask], BanMask),
	std::make_pair(CHANNEL_MODES[KeyProtected], KeyProtected),
	std::make_pair(CHANNEL_MODES[Limit], Limit),
	std::make_pair(CHANNEL_MODES[ChannelOperator], ChannelOperator),
};
static size_t const raw_channel_modes_len = sizeof(raw_channel_modes) / sizeof(*raw_channel_modes);

static std::map<char, ChannelMode> const channel_modes(raw_channel_modes, raw_channel_modes + raw_channel_modes_len);

typedef std::map<char, ChannelMode>::const_iterator ChannelModeIterator;
