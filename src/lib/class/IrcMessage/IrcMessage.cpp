/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:09:38 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 19:57:14 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"
#include <iostream>
#include <sstream>

IrcMessage::IrcMessage() {}
IrcMessage::~IrcMessage() {}

// TODO use this instead of IrcMessage variable (because it's a method)
IrcMessage IrcMessage::parse_irc_message(std::string const &message)
{
	IrcMessage         ircMessage;
	std::istringstream iss(message);
	std::string        token;

	if (message[0] == ':')
	{
		std::getline(iss, token, ' ');
		ircMessage._prefix = token.substr(1); // Remove the leading ':'
	}

	std::getline(iss, token, ' ');
	ircMessage._command = token;

	std::string remaining;

	std::getline(iss, remaining);

	size_t pos = remaining.find(':');

	if (pos != std::string::npos)
	{
		if (pos > 0)
		{
			ircMessage._params.push_back(remaining.substr(0, pos));
		}
		ircMessage._params.push_back(remaining.substr(pos));
	}
	else
	{
		if (!remaining.empty())
		{
			ircMessage._params.push_back(remaining);
		}
	}

	if (message.size() >= 2 && message.substr(message.size() - 2) == "\r\n")
	{
		ircMessage._end = "\r\n";
		std::string &lastString = ircMessage._params.back();
		lastString.erase(lastString.length() - 2);
	}

	return ircMessage;
}
