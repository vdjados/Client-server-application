#pragma once

#include <string>
#include "SocketWrapper.h"

class Client {
public:
    Client();
    ~Client();

    bool connectToServer(const std::string& ip, unsigned short port);

    std::string sendAndReceive(const std::string& message);

private:
    SocketWrapper clientSocket;
};
