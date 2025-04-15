#pragma once

#include <string>
#include "SocketWrapper.h"

class Client {
public:
    Client();
    ~Client();

    // Подключение к серверу по IP и порту
    bool connectToServer(const std::string& ip, unsigned short port);

    // Отправка сообщения серверу и получение ответа
    std::string sendAndReceive(const std::string& message);

private:
    SocketWrapper clientSocket;
};
