/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:36:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 15:24:41 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

enum UserMode
{
	Bot,
	LocalOperator,
	Away,
	Invisible,
	WallopsListener,
	EndOfUserModes,
	AlreadySentPass,
	AlreadySentUser
};
