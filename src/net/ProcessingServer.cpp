#include "net/ProcessingServer.h"
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>

ProcessingServer::ProcessingServer() {
    displaySocket.init();
}

ProcessingServer::~ProcessingServer() {
    displaySocket.close();
}

void ProcessingServer::run(unsigned short listenPort,
                           const std::string& displayIp,
                           unsigned short displayPort)
{
    // 1) Устанавливаем соединение с Display‑сервером
    if (!connectToDisplay(displayIp, displayPort)) {
        return;  // выходим, если не удалось
    }

    // 2) Делаем всю серверную работу родительского класса
    ServerBase::run(listenPort);
}

bool ProcessingServer::connectToDisplay(const std::string& ip,
                                        unsigned short port)
{
    if (!displaySocket.create()) {
        std::cerr << "ProcessingServer: cannot create display socket\n";
        return false;
    }
    if (!displaySocket.connect(ip, port)) {
        std::cerr << "ProcessingServer: cannot connect to DisplayServer at "
                  << ip << ":" << port << "\n";
        return false;
    }
    std::cout << "ProcessingServer: connected to DisplayServer at "
              << ip << ":" << port << "\n";
    return true;
}

void ProcessingServer::handleClient(SocketWrapper& clientSocket) {
    std::string input;
    while (clientSocket.receive(input) > 0) {
        std::cout << "ProcessingServer: Received: " << input << "\n";

        if (!isValidData(input)) {
            clientSocket.send("Invalid input.");
            continue;
        }

        clientSocket.send("Data received.");

        std::string result = processData(input);
        std::cout << "ProcessingServer: Processed: " << result << "\n";

        displaySocket.send(result);
    }
}

bool ProcessingServer::isValidData(const std::string& input) {
    return !input.empty();
}

std::string ProcessingServer::processData(const std::string& input) {
    std::istringstream iss(input);
    std::set<std::string> uniq;
    std::string w;
    while (iss >> w) uniq.insert(w);
    std::ostringstream oss;
    for (auto& s : uniq) oss << s << " ";
    auto s = oss.str();
    if (!s.empty()) s.pop_back();
    return s;
}
