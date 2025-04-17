#pragma once

#include "SocketWrapper.h"

class ServerBase {
public:
    ServerBase();
    virtual ~ServerBase();

    bool run(unsigned short port);

protected:
    virtual void handleClient(SocketWrapper& clientSocket) = 0;

    SocketWrapper serverSocket;
};
