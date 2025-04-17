#pragma once

#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET SocketType;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketHandle;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1
#endif

class SocketWrapper {
public:
    SocketWrapper();
    ~SocketWrapper();

    static bool init();                  
    void cleanup();
    bool create();                       // socket()
    bool bind(unsigned short port);     // bind()
    bool listen();                       // listen()
    SocketWrapper accept();             // accept()
    bool connect(const std::string& ip, unsigned short port);  // connect()
    int send(const std::string& data);  // send()
    int receive(std::string& data);     // recv()
    void close();                       // закрытие
    bool isValid() const;

    SocketWrapper(const SocketWrapper&) = delete; // Запрет копирования
    SocketWrapper& operator=(const SocketWrapper&) = delete;
    
    SocketWrapper(SocketWrapper&& other) noexcept; // Конструктор перемещения
    SocketWrapper& operator=(SocketWrapper&& other) noexcept; 
private:
    SocketType sock;

    #ifdef _WIN32
        static int wsaCount;
        bool inited_;  
    #endif
};
