/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:29:14 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:11:08 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
