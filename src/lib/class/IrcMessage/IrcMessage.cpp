#include "IrcMessage.hpp"

void IrcMessage::parseSingleMessage(const std::string& msg) {
    size_t pos = 0, spacePos = 0;

    // Prefix
    if (msg[pos] == ':') {
        spacePos = msg.find(' ', pos);
        this->setPrefix(msg.substr(pos + 1, spacePos - pos - 1));
        pos = spacePos + 1;
    }

    // Command
    spacePos = msg.find(' ', pos);
    if (spacePos != std::string::npos) {
        this->setCommand(msg.substr(pos, spacePos - pos));
        pos = spacePos + 1;
    } else {
        this->setCommand(msg.substr(pos));
        return;
    }

    // Params
    while ((spacePos = msg.find(' ', pos)) != std::string::npos) {
        if (msg[pos] == ':') {
            this->addParam(msg.substr(pos + 1));
            break;
        } else {
            this->addParam(msg.substr(pos, spacePos - pos));
            pos = spacePos + 1;
        }
    }

    // Last param without space
    if (pos < msg.length() && msg[pos] != ':') {
        this->addParam(msg.substr(pos));
    }
}
