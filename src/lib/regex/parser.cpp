#include <iostream>
#include <string>

class Command {
public:
    std::string name;
    std::string arguments;
};

class Client {
public:
    std::string nickname;
    std::string message;
};

class Parser {
public:
    Parser(const std::string& data) : data(data), index(0) {}

    Command parseCommand() {
        Command command;
        command.name = parseIdentifier();
        command.arguments = parseArguments();
        return command;
    }

    Client parseClient() {
        Client client;
        client.nickname = parseIdentifier();
        client.message = parseMessage();
        return client;
    }

private:
    std::string parseIdentifier() {
        std::string identifier;
        while (index < data.length() && isalnum(data[index])) {
            identifier += data[index++];
        }
        return identifier;
    }

    std::string parseArguments() {
        std::string arguments;
        if (data[index] == ' ') {
            index++;
            while (index < data.length() && data[index] != ':') {
                arguments += data[index++];
            }
        }
        return arguments;
    }

    std::string parseMessage() {
        std::string message;
        if (data[index] == ':') {
            index++;
            while (index < data.length()) {
                message += data[index++];
            }
        }
        return message;
    }

    std::string data;
    size_t index;
};

int main() {
    std::string inputData = "COMMAND arg1 arg2";
    Parser parser(inputData);
    Command command = parser.parseCommand();
    std::cout << "Command Name: " << command.name << std::endl;
    std::cout << "Command Arguments: " << command.arguments << std::endl;

    std::string inputData2 = "CLIENT:Hello, World!";
    Parser parser2(inputData2);
    Client client = parser2.parseClient();
    std::cout << "Client Nickname: " << client.nickname << std::endl;
    std::cout << "Client Message: " << client.message << std::endl;

    return 0;
}
