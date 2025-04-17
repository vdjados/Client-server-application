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
    bool create();                       
    bool bind(unsigned short port);     
    bool listen();                      
    SocketWrapper accept();            
    bool connect(const std::string& ip, unsigned short port);  
    int send(const std::string& data);  
    int receive(std::string& data);     
    void close();                     
    bool isValid() const;

    SocketWrapper(const SocketWrapper&) = delete; 
    SocketWrapper& operator=(const SocketWrapper&) = delete;
    
    SocketWrapper(SocketWrapper&& other) noexcept; 
    SocketWrapper& operator=(SocketWrapper&& other) noexcept; 
private:
    SocketType sock;

    #ifdef _WIN32
        static int wsaCount;
        bool inited_;  
    #endif
};
