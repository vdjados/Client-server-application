#include "net/Client.h"
#include <iostream>

int main() {
    Client client;
    // Подключаемся к серверу (127.0.0.1, порт 12345)
    if (client.connectToServer("127.0.0.1", 54000)) {
        std::cout << "Enter message (exit to interrupt connection):\n";
        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input == "exit") break;
            std::string response = client.sendAndReceive(input);
            std::cout << "Server answer: " << response << "\n";
        }
    }
    return 0;
}
