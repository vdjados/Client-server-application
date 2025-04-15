#include "net/Server.h"
#include <iostream>
#include <algorithm>

Server::Server() {
    // Инициализируем сокетную подсистему (если Windows)
    serverSocket.init();
}

Server::~Server() {
    // Деструктор закроет сокет через SocketWrapper
}

bool Server::start(unsigned short port) {
    if (!serverSocket.create()) {
        std::cerr << "Server: failed to create socket.\n";
        return false;
    }

    if (!serverSocket.bind(port)) {
        std::cerr << "Server: bind() failed.\n";
        return false;
    }

    if (!serverSocket.listen()) {
        std::cerr << "Server: listen() failed.\n";
        return false;
    }

    std::cout << "Server: served started and listening the port " << port << "\n";
    return true;
}

void Server::run() {
    // Ждем подключения клиента
    SocketWrapper clientSocket = serverSocket.accept();
    if (!clientSocket.isValid()) {
        std::cerr << "Server: accept() failed.\n";
        return;
    }
    std::cout << "Server: client connected.\n";

    std::string received;
    while (clientSocket.receive(received) > 0) {
        std::cout << "Server: message recieved: " << received << "\n";
        std::string response = processData(received);
        clientSocket.send(response);
    }
    std::cout << "Server: client disconnected.\n";
    clientSocket.close();
}

std::string Server::processData(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
