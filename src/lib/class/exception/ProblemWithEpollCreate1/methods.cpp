/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 01:00:12 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:00:20 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithEpollCreate1.hpp"

char const *ProblemWithEpollCreate1::what(void) const throw() { return "Problem with epoll_create1()"; }
