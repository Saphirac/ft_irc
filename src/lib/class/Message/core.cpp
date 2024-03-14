/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 00:14:13 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Message.hpp"

// Constructors //

/**
 * @brief Parses a message as a string to extract the prefix, the command, and the parameters from it.
 *
 * @param msg The message string to parse.
 *
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
Message::Message(std::string const &msg) : _prefix(), _command(), _parameters()
{
	size_t pos = 0;
	size_t space_pos;

	// Prefix
	if (msg[pos] == ':')
	{
		space_pos = msg.find(' ', pos);
		this->_prefix = msg.substr(pos, space_pos - pos);
		pos = space_pos + 1;
	}

	// Command
	if ((space_pos = msg.find(' ', pos)) == std::string::npos)
	{
		this->_command = msg.substr(pos);
		return;
	}
	this->_command = msg.substr(pos, space_pos - pos);
	pos = space_pos + 1;

	size_t const length = msg.length();

	// Parameters
	while (pos < length)
	{
		if (msg[pos] == ':')
		{
			this->_parameters.push_back(msg.substr(pos + 1));
			return;
		}
		if ((space_pos = msg.find(' ', pos)) == std::string::npos)
		{
			this->_parameters.push_back(msg.substr(pos));
			return;
		}
		this->_parameters.push_back(msg.substr(pos, space_pos - pos));
		pos = space_pos + 1;
	}
}

// Destructors //

Message::~Message(void) {}

// Accessors //

std::string const                    &Message::get_prefix(void) const { return this->_prefix; }
std::string const                    &Message::get_command(void) const { return this->_command; }
Server::CommandParameterVector const &Message::get_parameters(void) const { return this->_parameters; }
