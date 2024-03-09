/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 00:59:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 00:59:34 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithClock.hpp"

char const *ProblemWithClock::what(void) const throw() { return "Problem with clock()"; }
