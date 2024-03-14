/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 03:07:44 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 03:10:15 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/UserNotOnChannel.hpp"

char const *UserNotOnChannel::what(void) const throw() { return "The given user is not a channel member"; }
