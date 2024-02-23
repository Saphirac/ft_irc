/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:31:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/08 01:02:25 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMODE_HPP
#define CHANNELMODE_HPP

enum ChannelMode
{
	InviteList,
	BanList,
	InviteOnly,
	Key,
	Limit,
	NoMessagesFromOutside,
	ChannelOperator,
	RestrictedTopic,
	EndOfChannelModes
};

#endif
