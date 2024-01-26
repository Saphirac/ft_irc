/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 03:39:17 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:08:25 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Server.hpp"

class Server;

/**
 * @brief class command to stock the received client command
 * 
 * //TODO : create a map / lookup table + static method to stock all commands
 * 
 */
class Command
{
private:
	std::string	_cmd;
	std::string	_args;
	std::string	_prefix;
	std::string	_full;
	Server		*_server;

public:

	Command(std::string const cmd, std::string const args, std::string const prefix, std::string const full, Server *server);
	~Command();
	
	std::string	getCmd() const;
	std::string	getArgs() const;
	std::string	getPrefix() const;
	std::string	getTrailing() const;
	std::string	getFull() const;
	Server		*getServer() const;
	
	void		setCmd(std::string const cmd);
	void		setArgs(std::string const args);
	void		setPrefix(std::string const prefix);
	void		setFull(std::string const full);
	void		setServer(Server *server);
};
