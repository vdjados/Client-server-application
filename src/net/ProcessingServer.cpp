#include "net/ProcessingServer.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <algorithm>

#include "ext/json.hpp"
using json = nlohmann::json;

ProcessingServer::ProcessingServer() {
    displaySocket.init();
}

ProcessingServer::~ProcessingServer() {
    displaySocket.close();
}

bool ProcessingServer::run(unsigned short listenPort,
                           const std::string& displayIp,
                           unsigned short displayPort)
{
    if (!connectToDisplay(displayIp, displayPort)) {
        return false;
    }
    return ServerBase::run(listenPort);
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

        json j;
        j["result"] = result;
        std::string jsonStr = j.dump();

        std::ostringstream req;
        req << "POST / HTTP/1.1\r\n";
        req << "Host: display.server\r\n";
        req << "Content-Type: application/json\r\n";
        req << "Content-Length: " << jsonStr.size() << "\r\n";
        req << "\r\n";
        req << jsonStr;

        std::string request = req.str();

        displaySocket.send(request);
    }
}

bool ProcessingServer::isValidData(const std::string& input) {
    return !input.empty();
}

std::string ProcessingServer::processData(const std::string& input) {
    std::unordered_set<std::string> seen;
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        if (seen.insert(word).second) {
            result.push_back(word); 
        }
    }

    std::ostringstream oss;
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) oss << " ";
        oss << result[i];
    }

    return oss.str();
}