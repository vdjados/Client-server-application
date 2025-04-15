#include "net/SocketWrapper.h"
#include <iostream>

SocketWrapper::SocketWrapper() : sock(INVALID_SOCKET) {}

SocketWrapper::~SocketWrapper() {
    close();
    #ifdef _WIN32
        WSACleanup();
    #endif
}

bool SocketWrapper::init() {
    #ifdef _WIN32
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
    #else
        return true;
    #endif
}

bool SocketWrapper::create() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    return isValid();
}

bool SocketWrapper::bind(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return ::bind(sock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool SocketWrapper::listen() {
    return ::listen(sock, 5) != SOCKET_ERROR;
}

SocketWrapper SocketWrapper::accept() {
    SocketWrapper client;
    sockaddr_in clientAddr{};
#ifdef _WIN32
    int addrLen = sizeof(clientAddr);
#else
    socklen_t addrLen = sizeof(clientAddr);
#endif
    client.sock = ::accept(sock, (sockaddr*)&clientAddr, &addrLen);
    return client;
}

bool SocketWrapper::connect(const std::string& ip, unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    #ifdef _WIN32
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
    #else
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            std::cerr << "Ошибка преобразования IP адреса\n";
            return false;
        }
    #endif
    return ::connect(sock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

int SocketWrapper::send(const std::string& data) {
    return ::send(sock, data.c_str(), static_cast<int>(data.size()), 0);
}

int SocketWrapper::receive(std::string& data) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int received = ::recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        data = buffer;
    }
    return received;
}

void SocketWrapper::close() {
    if (isValid()) {
        #ifdef _WIN32
                closesocket(sock);
        #else
                ::close(sock);
        #endif
                sock = INVALID_SOCKET;
    }
}

bool SocketWrapper::isValid() const {
    return sock != INVALID_SOCKET;
}
