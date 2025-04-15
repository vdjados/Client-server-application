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

    bool init();                         // Инициализация (только Windows)
    bool create();                       // socket()
    bool bind(unsigned short port);     // bind()
    bool listen();                       // listen()
    SocketWrapper accept();             // accept()
    bool connect(const std::string& ip, unsigned short port);  // connect()
    int send(const std::string& data);  // send()
    int receive(std::string& data);     // recv()
    void close();                       // закрытие
    bool isValid() const;

private:
    SocketType sock;
};
