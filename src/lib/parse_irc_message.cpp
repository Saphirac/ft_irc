#include "IrcMessage.hpp"
#include "ft_irc.hpp"

IrcMessage parse_irc_message(const std::string &message)
{
	IrcMessage  ircMessage;
	std::string temp;
	size_t      start = 0, end;

	if (message[start] == ':')
	{
		end = message.find(' ', start);
		ircMessage.set_prefix(message.substr(1, end - 1));
		start = end + 1;
	}

	end = message.find(' ', start);
	ircMessage.set_command(message.substr(start, end - start));
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
			ircMessage.get_params()->push_back(temp.substr(1));
		}
		else
		{
			ircMessage.get_params()->push_back(temp);
		}
		start = end + 1;
	}

	if (!lastParam && start < message.length())
	{
		ircMessage.get_params()->push_back(message.substr(start));
	}

	return ircMessage;
}
