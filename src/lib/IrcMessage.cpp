#include "IrcMessage.hpp"

void IrcMessage::display() const
{
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::cout << "Params: ";
	for (size_t i = 0; i < _params.size(); ++i)
	{
		std::cout << "[" << _params[i] << "]";
	}
	std::cout << std::endl;
	std::cout << "End: " << (_end.empty() ? "null" : "\\r\\n") << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}
IrcMessage IrcMessage::parseIrcMessage(std::string const &message)
{
	IrcMessage ircMessage;
	std::istringstream iss(message);
	std::string token;

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

        std::string& lastString = ircMessage._params.back();
        lastString.erase(lastString.length() - 2);
	}

	return ircMessage;
}
