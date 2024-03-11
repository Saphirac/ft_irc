/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/11 02:26:24 by mcourtoi         ###   ########.fr       */
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

	size_t const length = msg.length();

	// Parameters
	while (pos < length && (space_pos = msg.find(' ', pos)) != std::string::npos)
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

	if (pos < length)
	{
		this->_parameters.push_back(msg[pos] == ':' ? msg.substr(pos) : msg.substr(pos));
	}
}

// Destructors //

Message::~Message() {}

// Accessors //

std::string const              &Message::get_prefix(void) const { return this->_prefix; }
std::string const              &Message::get_command(void) const { return this->_command; }
std::vector<std::string> const &Message::get_parameters(void) const { return this->_parameters; }
