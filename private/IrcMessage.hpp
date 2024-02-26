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

#endif
