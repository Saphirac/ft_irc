/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:31:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 14:39:49 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define CHANNEL_MODES "intlkoIb" // The order of the characters must match the order of the enum ChannelMode

enum ChannelMode
{
	InviteOnly,            // 'i'
	NoMessagesFromOutside, // 'n'
	RestrictedTopic,       // 't'
	Limit,                 // 'l'
	KeyProtected,          // 'k'
	ChannelOperator,       // 'o'
	InviteMask,            // 'I'
	BanMask                // 'b'
};
