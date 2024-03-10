/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/04 22:03:52 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Message.hpp"

// Constructors //

/**
 * @param raw_msg The raw message to parse and get the prefix, the command, and the arguments from.
 */
Message::Message(std::string const &raw_msg) : _prefix(), _command(), _parameters()
{
	size_t pos = 0;
	size_t space_pos = 0;

	// Prefix
	if (raw_msg[pos] == ':')
	{
		space_pos = raw_msg.find(' ', pos);
		this->_prefix = raw_msg.substr(pos + 1, space_pos - pos - 1);
		pos = space_pos + 1;
	}

	// Command
	space_pos = raw_msg.find(' ', pos);
	if (space_pos == std::string::npos)
	{
		this->_command = raw_msg.substr(pos);
		return;
	}
	this->_command = raw_msg.substr(pos, space_pos - pos);
	pos = space_pos + 1;

	// Params
	while ((space_pos = raw_msg.find(' ', pos)) != std::string::npos)
	{
		if (raw_msg[pos] == ':')
		{
			this->_parameters.push_back(raw_msg.substr(pos + 1));
			break;
		}
		this->_parameters.push_back(raw_msg.substr(pos, space_pos - pos));
		pos = space_pos + 1;
	}

	// Last param without space
	if (pos < raw_msg.length() && raw_msg[pos] != ':')
		this->_parameters.push_back(raw_msg.substr(pos));
}

// Destructor //

Message::~Message(void) {}

// Accessors //

std::string const              &Message::get_prefix(void) const { return this->_prefix; }
std::string const              &Message::get_command(void) const { return this->_command; }
std::vector<std::string> const &Message::get_parameters(void) const { return this->_parameters; }
