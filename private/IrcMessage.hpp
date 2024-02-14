#ifndef IRCMESSAGE_HPP
# define IRCMESSAGE_HPP

#include "irc.hpp"

class IrcMessage {
public:
    std::string prefix;
    std::string command;
    std::vector<std::string> params;

    void display() const;
};

#endif
