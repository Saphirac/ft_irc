#include "IrcMessage.hpp"

void IrcMessage::display() const 
{
    std::cout << "Prefix: " << prefix << std::endl;
    std::cout << "Command: " << command << std::endl;
    std::cout << "Params:" << std::endl;
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << "  " << params[i] << std::endl;
    }
}
