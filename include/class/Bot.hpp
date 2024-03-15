/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:56:18 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/15 01:34:13 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Message.hpp"
#include <sys/select.h>
#include <string>
#include <map>

#define DEFAULT_IRC_PORT 6667

class Bot
{
public:

	Bot(int const port = DEFAULT_IRC_PORT, std::string const &password = "");

	~Bot();

	void run();

private:

	typedef void (Bot::*Command)(Message const &msg);
	typedef std::map<std::string, Command const> _CommandMap;
	typedef std::pair<std::string const, Command const> _CommandPair;

	static _CommandPair const _raw_commands_by_name[];
	static _CommandMap const  _commands_by_name;


	int const         _socket;
	std::string const _password;
	fd_set            _read_fds;
	std::string       _msg_in;


	void        _append_to_msg_in(std::string const &s);
	std::string _get_next_msg(void);
	
	void _send_connexion_message();
	void _ping(Message const &msg);
	void _privmsg(Message const &msg);
	void _invite(Message const &msg);
	void _pass(Message const &msg);

	void _disconnect();

	void _bot_routine(fd_set &read_fds, int &max_fd, timeval &timeout);
};
