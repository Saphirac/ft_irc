/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 19:01:27 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/13 19:12:10 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include "class/exception/ProblemWithSend.hpp"
#include "class/exception/ProblemWithRecv.hpp"

void Bot::_send_connexion_message()
{
	std::string const connexion_msg = "PASS " + password + "\r\n" + "NICK Wall-E\r\n" + "USER userbot 0 localhost userbot\r\n" + "MODE +B\r\n";

	if (send(this->sockfd, connexion_msg.c_str(), connexion_msg.size(), 0) < 0) 
		throw ProblemWithSend();
}

inline static void Bot::_privmsg_cmd(std::string &response, Message const &msg)
{
	if (msg.get_parameters().size() < 2)
		return ;

	std::string const scd_param = remove_crlf(msg.get_parameters()[1]);

	if (!scd_param.empty()) 
	{
		if (scd_param == "foo")
			response = "PRIVMSG " + msg.get_prefix().who_is_sender() + " bar\r\n";
		else if (scd_param == "Eve")
			response = "PRIVMSG " + msg.get_prefix().who_is_sender() + " Wall-E\r\n";
	}
}

inline static void Bot::_ping_cmd(std::string &response, Message const &msg)
{
	std::string const scd_param = remove_crlf(msg.get_parameters()[1]);

	response = "PONG " + msg.get_parameters()[0];
}

inline static void Bot::_join_cmd(std::string &response, Message const &msg)
{
	std::cout << "Joining [" << msg.get_parameters()[1] << "]" << std::endl;
	response = "JOIN " + msg.get_parameters()[1];
}

void _bot_loop()
{
	while (run_bool == true)
	{
		char buffer[4096];
		ssize_t received = recv(this->socket, buffer, sizeof(buffer) - 1, 0);
		buffer[received] = '\0'; // Assurez-vous que la chaîne est correctement terminée.

		if (received < 0)
			throw ProblemWithRecv();
		else if (received == 0) 
		{
			std::cout << "Server closed connection" << std::endl;
			return ;
		} 
		else 
		{
			Message msg((std::string(buffer, received)));
			std::string response;

			std::cout << "Command : [" << msg.get_command() << "]" << std::endl;

			if (msg.get_command() == "PRIVMSG")
				privmsg_cmd(response, msg);
			if (msg.get_command() == "PING")
				ping_cmd(response, msg);
			if (msg.get_command() == "INVITE")
				join_cmd(response, msg);
			if (!response.empty() && send(this->socket, response.c_str(), response.size(), 0) < 0)
				throw ProblemWithSend();
		}
	}
}

void Bot::run()
{
	
}
