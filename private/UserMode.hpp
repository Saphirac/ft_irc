/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:29:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/02 17:59:04 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define USER_MODES "BOwia" // The order of the characters must match the order of the enum UserMode

enum UserMode
{
	Bot,             // 'B'
	LocalOperator,   // 'O'
	WallopsListener, // 'w'
	Invisible,       // 'i'
	Away,            // 'a'
	AlreadySentPass,
	AlreadySentUser
};
