#include "Message.hpp"

/**
 * Constructor for the Message class.
 * Parses a single message from the given string.
 *
 * @param msg A constant reference to a std::string containing the message to be parsed.
 *
 * @return A new instance of the Message class.
 */
void Message::parseSingleMessage(const std::string &msg)
{
	size_t pos = 0;
	size_t spacePos = 0;

	// Prefix
	if (msg[pos] == ':')
	{
		spacePos = msg.find(' ', pos);
		this->set_prefix(msg.substr(pos + 1, spacePos - pos - 1));
		pos = spacePos + 1;
	}

	// Command
	spacePos = msg.find(' ', pos);
	if (spacePos != std::string::npos)
	{
		this->set_command(msg.substr(pos, spacePos - pos));
		pos = spacePos + 1;
	}
	else
	{
		this->set_command(msg.substr(pos));
		return;
	}

	// Params
	while ((spacePos = msg.find(' ', pos)) != std::string::npos)
	{
		if (msg[pos] == ':')
		{
			this->add_param(msg.substr(pos + 1));
			break;
		}
		else
		{
			this->add_param(msg.substr(pos, spacePos - pos));
			pos = spacePos + 1;
		}
	}

	// Last param without space
	if (pos < msg.length() && msg[pos] != ':')
	{
		this->add_param(msg.substr(pos));
	}
}
