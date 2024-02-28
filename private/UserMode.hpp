/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:29:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 11:17:09 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define USER_MODES "BOaiw" // The order of the characters must match the order of the enum UserMode

enum UserMode
{
	Bot,             // 'B'
	LocalOperator,   // 'O'
	Away,            // 'a'
	Invisible,       // 'i'
	WallopsListener, // 'w'
	AlreadySentPass,
	AlreadySentUser
};
// REMIND: Make sure to keep EndOfUserModes up to date if you add or remove a mode
