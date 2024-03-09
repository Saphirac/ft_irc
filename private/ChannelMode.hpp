/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:31:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:20:33 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define CHANNEL_MODES "intlkoIb" // The order of the characters must match the order of the enum ChannelMode

// It is important to list the flags first to be able to use a bit field for them without wasting any bit.
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
