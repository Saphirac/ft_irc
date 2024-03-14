/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:56:18 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 18:08:14 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Message.hpp"
#include <sys/select.h>
#include <string>
#include <map>

class Bot
{
public:
	Bot(int const port = 0, std::string const &password = "");

	~Bot();

	void run();

private:

	typedef void (Bot::*Command)(Message const &msg);
	typedef std::map<std::string, Command const> CommandMap;
	typedef std::pair<std::string const, Command const> CommandPair;
	typedef CommandMap::const_iterator                  CommandIterator;

	static CommandPair const _raw_commands_by_name[];
	static CommandMap const  _commands_by_name;


	int         _socket;
	std::string _password;
	fd_set   	_read_fds;

	void _send_connexion_message();
	void _ping(Message const &msg);
	void _privmsg(Message const &msg);
	void _invite(Message const &msg);
	void _disconnect();

	void _bot_routine(fd_set &read_fds, int &max_fd, timeval &timeout);
};
