/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 04:05:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:08:38 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(std::string const cmd, std::string const args, std::string const prefix, std::string const full, Server *server) :
_cmd(cmd),
_args(args),
_prefix(prefix),
_full(full),
_server(server)
{
	if (DEBUG)
		std::cout << "Command constructor called\n";
}

Command::~Command()
{
	if (DEBUG)
		std::cout << "Command destructor called\n";
}

// Getters //

std::string	Command::getCmd() const
{
	if (DEBUG)
		std::cout << "getCmd() member function of Command called\n";
	return this->_cmd;
}

std::string	Command::getArgs() const
{
	if (DEBUG)
		std::cout << "getArgs() member function of Command called\n";
	return this->_args;
}

std::string	Command::getPrefix() const
{
	if (DEBUG)
		std::cout << "getPrefix() member function of Command called\n";
	return this->_prefix;
}

std::string	Command::getFull() const
{
	if (DEBUG)
		std::cout << "getFull() member function of Command called\n";
	return this->_full;
}

Server	*Command::getServer() const
{
	if (DEBUG)
		std::cout << "getServer() member function of Command called\n";
	return this->_server;
}

// Setters //

void	Command::setCmd(std::string const cmd)
{
	if (DEBUG)
		std::cout << "setCmd() member function of Command called\n";
	this->_cmd = cmd;
}

void	Command::setArgs(std::string const args)
{
	if (DEBUG)
		std::cout << "setArgs() member function of Command called\n";
	this->_args = args;
}

void	Command::setPrefix(std::string const prefix)
{
	if (DEBUG)
		std::cout << "setPrefix() member function of Command called\n";
	this->_prefix = prefix;
}

void	Command::setFull(std::string const full)
{
	if (DEBUG)
		std::cout << "setFull() member function of Command called\n";
	this->_full = full;
}

void	Command::setServer(Server *server)
{
	if (DEBUG)
		std::cout << "setServer() member function of Command called\n";
	this->_server = server;
}
