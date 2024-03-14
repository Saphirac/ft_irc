/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:56:18 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 03:58:47 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Message.hpp"
#include <string>

class Bot
{
public:

	Bot(int const port = 0, std::string const &password = "");

	~Bot();
	
	void run();

private:

	int _socket;
	std::string _password;

	void _send_connexion_message();
	void _ping_cmd(std::string &response, Message const &msg);
	void _privmsg_cmd(std::string &response, Message const &msg);
	void _join_cmd(std::string &response, Message const &msg);
	void _disconnect();

	void _bot_loop();
};
