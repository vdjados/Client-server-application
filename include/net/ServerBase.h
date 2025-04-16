#pragma once

#include "SocketWrapper.h"

class ServerBase {
public:
    ServerBase();
    virtual ~ServerBase();

    void run(unsigned short port);  // Запускает сервер и вызывает handleClient

protected:
    virtual void handleClient(SocketWrapper& clientSocket) = 0;  // Реализуется в потомке

    SocketWrapper serverSocket;
};
