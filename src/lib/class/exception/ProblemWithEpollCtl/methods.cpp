/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 01:00:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:00:58 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithEpollCtl.hpp"

char const *ProblemWithEpollCtl::what(void) const throw() { return "Problem with epoll_ctl()"; }
