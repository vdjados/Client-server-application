#include "net/Client.h"
#include <iostream>
#include <string>
#include <cstdlib>  // для std::stoi

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>\n";
        return 1;
    }

    std::string serverIp = argv[1];
    unsigned short serverPort = static_cast<unsigned short>(std::stoi(argv[2]));

    Client client;
    if (client.connectToServer(serverIp, serverPort)) {
        std::cout << "Enter message (type 'exit' to quit):\n";
        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input == "exit") break;

            std::string response = client.sendAndReceive(input);
            std::cout << "Server answer: " << response << "\n";
        }
    } else{
        std::cerr << "Client: unable to connect → exiting\n";
        return 1;
    }

    return 0;
}