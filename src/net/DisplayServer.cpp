#include "net/DisplayServer.h"
#include <iostream>

void DisplayServer::handleClient(SocketWrapper& clientSocket) {
    std::string received;
    while (clientSocket.receive(received) > 0) {
        std::cout << "DisplayServer: Received data: " << received << "\n";
    }
    std::cout << "DisplayServer: Client disconnected.\n";
}
