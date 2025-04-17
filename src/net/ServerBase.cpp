#include "net/ServerBase.h"
#include <iostream>

ServerBase::ServerBase() {
    serverSocket.init();
}

ServerBase::~ServerBase() {
    
}

bool ServerBase::run(unsigned short port) {
    if (!serverSocket.create()) {
        std::cerr << "ServerBase: Failed to create socket.\n";
        return false;
    }

    if (!serverSocket.bind(port)) {
        std::cerr << "ServerBase: Failed to bind on port " << port << ".\n";
        return false;
    }

    if (!serverSocket.listen()) {
        std::cerr << "ServerBase: Failed to listen on port.\n";
        return false;
    }

    std::cout << "ServerBase: Listening on port " << port << "...\n";

    SocketWrapper client = serverSocket.accept();
    if (!client.isValid()) {
        std::cerr << "ServerBase: Failed to accept connection.\n";
        return false;
    }

    std::cout << "ServerBase: Client connected.\n";

    handleClient(client);
    return true;
}
