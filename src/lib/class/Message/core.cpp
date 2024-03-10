/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/10 03:35:13 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Message.hpp"

/**
 * Constructor for the Message class.
 * Parses a single message from the given string.
 *
 * @param msg A constant reference to a std::string containing the message to be parsed.
 *
 * @return A new instance of the Message class.
 */
Message::Message(std::string const &msg)
{
	size_t pos = 0;
	size_t space_pos = 0;

	// Prefix
	if (msg[pos] == ':')
	{
		space_pos = msg.find(' ', pos);
		this->_prefix = msg.substr(pos + 1, space_pos - pos - 1);
		pos = space_pos + 1;
	}

	// Command
	space_pos = msg.find(' ', pos);
	if (space_pos != std::string::npos)
	{
		this->_command = msg.substr(pos, space_pos - pos);
		pos = space_pos + 1;
	}
	else
	{
		this->_command = msg.substr(pos);
		return;
	}

	// Parameters
	while (pos < msg.length() && (space_pos = msg.find(' ', pos)) != std::string::npos)
	{
		if (msg[pos] == ':')
		{
			this->_parameters.push_back(msg.substr(pos));
			return;
		}
		else
		{
			this->_parameters.push_back(msg.substr(pos, space_pos - pos));
			pos = space_pos + 1;
		}
	}

	if (pos < msg.length())
	{
		this->_parameters.push_back(msg[pos] == ':' ? msg.substr(pos) : msg.substr(pos));
	}
}

/**
 * Destructor for the Message class.
 */
Message::~Message() {}

/**
 * Parses a single message from the given string.
 *
 * @param msg A constant reference to a std::string containing the message to be parsed.
 */
std::string const &Message::get_prefix() const { return _prefix; }

/**
 * Returns the command of the message.
 *
 * @return A constant reference to a std::string containing the command of the message.
 */
std::string const &Message::get_command() const { return _command; }

/**
 * Returns the parameters of the message.
 *
 * @return A reference to a vector of std::string containing the parameters of the message.
 */
std::vector<std::string> const &Message::get_parameters() const { return _parameters; }
