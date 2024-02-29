/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:32:32 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 18:09:16 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:28:34 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:29:27 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"

char const *ProblemWithSocket::what(void) const throw() { return "Problem with socket"; }

char const *ProblemWithSockAddr::what(void) const throw()
{
	return "Problem with SockAddr structure creation / assignation";
}

char const *ProblemWithRecv::what(void) const throw() { return "Problem with recv()"; }

char const *ProblemWithListen::what(void) const throw() { return "Problem with listen()"; }

char const *ProblemWithAccept::what(void) const throw() { return "Problem with accept()"; }

char const *ProblemWithFcntlFlags::what(void) const throw() { return "Problem with fcntl() flags"; }

char const *ProblemWithSettingNonBlock::what(void) const throw()
{
	return "Problem with setting socket to non-blocking mode";
}

char const *ProblemWithSend::what(void) const throw() { return "Problem with send()"; }

char const *ProblemWithEpollCtl::what(void) const throw() { return "Problem with epoll_ctl()"; }

char const *ProblemWithEpollCreate::what(void) const throw() { return "Problem with epoll_create1()"; }

char const *ProblemWithEpollWait::what(void) const throw() { return "Problem with epoll_wait()"; }
