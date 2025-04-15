#pragma once

#include <string>
#include "SocketWrapper.h"

class Server {
public:
    Server();
    ~Server();

    // Запускает сервер: создает сокет, биндует порт, начинает слушать.
    bool start(unsigned short port);

    // Ждёт подключения и обрабатывает данные клиента
    void run();

private:
    SocketWrapper serverSocket;
    // Пример обработки данных: перевод сообщения в верхний регистр
    std::string processData(const std::string& input);
};
