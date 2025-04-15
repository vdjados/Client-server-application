// Client.cpp
#include <iostream>
#include <cstring>  // Для memset и strlen
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketType;
    #define CLOSESOCKET closesocket
    #define GET_ERROR WSAGetLastError()
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketType;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSESOCKET close
    #define GET_ERROR errno
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
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Failed to create client socket.\n";
        return 1;
    }

    // 2. Настройка адреса сервера: 127.0.0.1 (локальный компьютер) и порт 54000
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
#ifdef _WIN32
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#else
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Wrong IP adress\n";
        return 1;
    }
#endif

    // 3. Подключаемся к серверу
    if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server\n";
        CLOSESOCKET(clientSock);
        #ifdef _WIN32
                WSACleanup();
        #endif
        return 1;
    }
    std::cout << "Client: connection is successful!\n";

    std::string userInput;
    char buffer[1024];

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);
        if (userInput == "exit") break;

        send(clientSock, userInput.c_str(), userInput.size(), 0);

        int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Answer: " << buffer << "\n";
        }
    }

    CLOSESOCKET(clientSock);
    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}
