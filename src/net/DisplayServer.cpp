#include "net/DisplayServer.h"
#include <iostream>
#include <ext/json.hpp>

void DisplayServer::handleClient(SocketWrapper& clientSocket) {
    std::string received;

    while (clientSocket.receive(received) > 0) {

        auto jsonStart = received.find("\r\n\r\n");
        if (jsonStart != std::string::npos) {
            std::string body = received.substr(jsonStart + 4);

            try {
                nlohmann::json j = nlohmann::json::parse(body);
                std::string result = j.value("result", "<no result>");
                std::cout << "DisplayServer: Recieved data: " << result << "\n";
            } catch (const std::exception& e) {
                std::cout << "DisplayServer: JSON parse error: " << e.what() << "\n";
            }
        } else {
            std::cout << "DisplayServer: Invalid HTTP message (no header/body separator).\n";
        }
    }

    std::cout << "DisplayServer: Client disconnected.\n";
}