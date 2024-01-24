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

    // Test cases for parseCommand()
    std::string inputData3 = "COMMAND";
    Parser parser3(inputData3);
    Command command2 = parser3.parseCommand();
    std::cout << "Command Name: " << command2.name << std::endl;
    std::cout << "Command Arguments: " << command2.arguments << std::endl;

    std::string inputData4 = "COMMAND arg1 arg2 arg3";
    Parser parser4(inputData4);
    Command command3 = parser4.parseCommand();
    std::cout << "Command Name: " << command3.name << std::endl;
    std::cout << "Command Arguments: " << command3.arguments << std::endl;

    // Test cases for parseClient()
    std::string inputData5 = "CLIENT:Hello";
    Parser parser5(inputData5);
    Client client2 = parser5.parseClient();
    std::cout << "Client Nickname: " << client2.nickname << std::endl;
    std::cout << "Client Message: " << client2.message << std::endl;

    std::string inputData6 = "CLIENT:Hello, World!";
    Parser parser6(inputData6);
    Client client3 = parser6.parseClient();
    std::cout << "Client Nickname: " << client3.nickname << std::endl;
    std::cout << "Client Message: " << client3.message << std::endl;

    return 0;
}