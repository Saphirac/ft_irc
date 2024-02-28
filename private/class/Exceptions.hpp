/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:28:34 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 16:44:58 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class ProblemWithSocket : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithSockAddr : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithRecv : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithListen : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithAccept : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithFcntlFlags : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithSettingNonBlock : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithSend : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithEpollCtl : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithEpollCreate : public std::exception
{
public:
	virtual char const *what(void) const throw();
};

class ProblemWithEpollWait : public std::exception
{
public:
	virtual char const *what(void) const throw();
};
