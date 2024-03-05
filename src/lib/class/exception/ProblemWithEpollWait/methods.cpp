/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 01:01:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 01:01:30 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/exception/ProblemWithEpollWait.hpp"

char const *ProblemWithEpollWait::what(void) const throw() { return "Problem with epoll_wait()"; }
