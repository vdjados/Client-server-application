#include "net/Server.h"

int main() {
    Server server;
    // Запускаем сервер на порту 12345
    if (server.start(12345)) {
        server.run();
    }
    return 0;
}
