/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 01:01:51 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 02:20:01 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithFcntl.hpp"

char const *ProblemWithFcntl::what(void) const throw() { return "Problem with fcntl()"; }
