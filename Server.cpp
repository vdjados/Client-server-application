// Server.cpp
#include <iostream>
#include <cstring>  // Для memset и strlen

#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET SocketType;
    #pragma comment(lib, "ws2_32.lib")  // Обязательно при компиляции под Windows!
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int SocketType;
#endif

int main() {
#ifdef _WIN32
    // Инициализация Winsock (только для Windows)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    // 1. Создаем сокет
    SocketType serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // 2. Настройка адреса: используем IPv4, порт 54000, любой IP-адрес на данном компьютере
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;             // IPv4
    serverAddr.sin_port = htons(54000);           // Порт (htons преобразует число в сетевой порядок байт)
    serverAddr.sin_addr.s_addr = INADDR_ANY;      // Принимаем подключения с любого IP

    // 3. Привязываем сокет к адресу
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to configure socket\n";
        return 1;
    }

    // 4. Начинаем слушать – говорим системе, что сокет готов принимать входящие подключения
    if (listen(serverSock, 5) < 0) {
        std::cerr << "Failed to listen socket\n";
        return 1;
    }
    std::cout << "Server is working, waiting for connection...\n";

    // 5. Принимаем подключение (блокирующий вызов)
    sockaddr_in clientAddr;
#ifdef _WIN32
    int clientAddrSize = sizeof(clientAddr);
#else
    socklen_t clientAddrSize = sizeof(clientAddr);
#endif
    SocketType clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSock < 0) {
        std::cerr << "Failed to get connection\n";
        return 1;
    }
    std::cout << "Client connected.\n";

    // 6. Принимаем данные от клиента
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));  // Обнуляем буфер
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to get Data or client disconnected\n";
    } else {
        std::cout << "Message recieved: " << buffer << "\n";
        // 7. Отправляем подтверждение (например, "Сообщение получено")
        const char* ack = "Message recieved";
        send(clientSock, ack, strlen(ack), 0);
    }

    // 8. Закрываем сокеты
#ifdef _WIN32
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
#else
    close(clientSock);
    close(serverSock);
#endif

    return 0;
}
