#include "net/Client.h"
#include <iostream>

Client::Client() {
    clientSocket.init();
}

Client::~Client() {
    
}

bool Client::connectToServer(const std::string& ip, unsigned short port) {
    if (!clientSocket.create()) {
        std::cerr << "Client: failed to create socket.\n";
        return false;
    }
    if (!clientSocket.connect(ip, port)) {
        std::cerr << "Client: failed to connect to server.\n";
        return false;
    }
    std::cout << "Client: connected to server.\n";
    return true;
}

std::string Client::sendAndReceive(const std::string& message) {
    if (clientSocket.send(message) == SOCKET_ERROR) {
        std::cerr << "Client: failed to send data.\n";
    }
    std::string response;
    if (clientSocket.receive(response) <= 0) {
        std::cerr << "Client: failed to recieve data or server disconnected.\n";
    }
    return response;
}
