/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:36:33 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/04 03:40:13 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMODE_HPP
#define USERMODE_HPP

enum UserMode
{
	UserModeBot,
	UserModeOperator,
	UserModeAway,
	UserModeInvisible,
	UserModeWallops,
	UserModePass,
};

#endif