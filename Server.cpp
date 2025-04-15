// Server.cpp
#include <iostream>
#include <algorithm>
#include <cstring>  // Для memset и strlen

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketType;
    #define CLOSESOCKET closesocket
    #define GET_ERROR WSAGetLastError()    
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    typedef int SocketType;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSESOCKET close
    #define GET_ERROR errno
#endif

std::string processData(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

bool setIPAddress(sockaddr_in& addr, const std::string& ipStr) {
    #ifdef _WIN32
        addr.sin_addr.s_addr = inet_addr(ipStr.c_str());
        return addr.sin_addr.s_addr != INADDR_NONE;
    #else
        return inet_pton(AF_INET, ipStr.c_str(), &addr.sin_addr) > 0;
    #endif
}

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
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // 2. Настройка адреса: используем IPv4, порт 54000, любой IP-адрес на данном компьютере
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;             // IPv4
    serverAddr.sin_port = htons(54000);           // Порт (htons преобразует число в сетевой порядок байт)
    if (!setIPAddress(serverAddr, "127.0.0.1")) {
        std::cerr << "Wrong IP adress\n";
        CLOSESOCKET(serverSock);
        return 1;
    }
    
    // 3. Привязываем сокет к адресу
    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed bind(): " << GET_ERROR << "\n";
        CLOSESOCKET(serverSock);
        return 1;
    }

    // 4. Начинаем слушать – говорим системе, что сокет готов принимать входящие подключения
    if (listen(serverSock, 1) == SOCKET_ERROR) {
        std::cerr << "Failed listen(): " << GET_ERROR << "\n";
        CLOSESOCKET(serverSock);
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
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Failed to get connection\n";
        return 1;
    }
    std::cout << "Client connected.\n";

    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::string received(buffer);

        std::cout << "Recieved: " << received << "\n";

        std::string response = processData(received);
        send(clientSock, response.c_str(), response.size(), 0);
    }


    CLOSESOCKET(clientSock);
    CLOSESOCKET(serverSock);
    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}
