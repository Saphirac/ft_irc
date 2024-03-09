/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:46:55 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:52:49 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/InvalidConversion.hpp"

char const *InvalidConversion::what(void) const throw() { return "Invalid conversion in format string"; }
