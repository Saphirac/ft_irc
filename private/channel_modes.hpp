/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 22:57:52 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/08 01:01:05 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_MODES_HPP
#define CHANNEL_MODES_HPP

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

#endif
