/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 01:05:13 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:05:18 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/UnknownReply.hpp"

char const *UnknownReply::what(void) const throw() { return "Unknown reply"; }
