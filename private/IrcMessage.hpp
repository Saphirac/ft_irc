/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/26 16:44:08 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include "irc.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class IrcMessage
{
private:
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;
	std::string              _end;

public:
	void       display() const;
	IrcMessage parseIrcMessage(std::string const &msg);
};

IrcMessage parse_irc_message(const std::string &message);

#endif
