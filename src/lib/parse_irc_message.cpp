#include "IrcMessage.hpp"
#include "irc.hpp"

IrcMessage parseIrcMessage(const std::string &message)
{
	IrcMessage  ircMessage;
	std::string temp;
	size_t      start = 0, end;

	if (message[start] == ':')
	{
		end = message.find(' ', start);
		ircMessage.prefix = message.substr(1, end - 1);
		start = end + 1;
	}

	end = message.find(' ', start);
	ircMessage.command = message.substr(start, end - start);
	start = end + 1;

	bool lastParam = false;
	while ((end = message.find(' ', start)) != std::string::npos && !lastParam)
	{
		if (message[start] == ':')
		{
			lastParam = true;
			end = message.length();
		}

		temp = message.substr(start, end - start);
		if (lastParam)
		{
			ircMessage.params.push_back(temp.substr(1));
		}
		else
		{
			ircMessage.params.push_back(temp);
		}
		start = end + 1;
	}

	if (!lastParam && start < message.length())
	{
		ircMessage.params.push_back(message.substr(start));
	}

	return ircMessage;
}
