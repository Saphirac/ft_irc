/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 22:57:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:12:15 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ChannelMode.hpp"
#include <cstddef>

static char const channel_modes[] = {
	[InviteList] = 'I',
	[BanList] = 'b',
	[InviteOnly] = 'i',
	[Key] = 'k',
	[Limit] = 'l',
	[NoMessagesFromOutside] = 'n',
	[ChannelOperator] = 'o',
	[RestrictedTopic] = 't',
	[EndOfChannelModes] = 0,
};
static size_t const channel_modes_len = sizeof(channel_modes) / sizeof(*channel_modes);
