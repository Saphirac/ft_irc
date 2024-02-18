/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:29:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/18 23:53:42 by jodufour         ###   ########.fr       */
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
