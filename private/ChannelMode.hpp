/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMode.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:31:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:00:16 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
