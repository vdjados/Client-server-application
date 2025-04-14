// Client.cpp
#include <iostream>
#include <cstring>  // Для memset и strlen
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET SocketType;
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketType;
#endif

int main() {
#ifdef _WIN32
    // Инициализация Winsock для Windows
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    // 1. Создаем сокет клиента
    SocketType clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        std::cerr << "Failed to create client socket.\n";
        return 1;
    }

    // 2. Настройка адреса сервера: 127.0.0.1 (локальный компьютер) и порт 54000
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
#ifdef _WIN32
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#else
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Неверный IP адрес\n";
        return 1;
    }
#endif

    // 3. Подключаемся к серверу
    if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }
    std::cout << "Client: connection is successful!\n";

    // 4. Отправляем данные серверу
    std::string message = "Hello, server!";
    send(clientSock, message.c_str(), message.size(), 0);
    std::cout << "Client: message was sent\n";

    // 5. Ждем и принимаем подтверждение от сервера
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        std::cout << "Client: server answered: " << buffer << "\n";
    } else {
        std::cerr << "Failed to recieve answer from server\n";
    }

    // 6. Закрываем сокет
#ifdef _WIN32
    closesocket(clientSock);
    WSACleanup();
#else
    close(clientSock);
#endif

    return 0;
}
