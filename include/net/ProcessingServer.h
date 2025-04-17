#pragma once

#include "ServerBase.h"
#include "SocketWrapper.h"
#include <string>

class ProcessingServer : public ServerBase {
public:
    ProcessingServer();
    ~ProcessingServer() override;

    bool run(unsigned short listenPort,
             const std::string& displayIp,
             unsigned short displayPort);

    void handleClient(SocketWrapper& clientSocket) override;

private:
    SocketWrapper displaySocket;

    bool connectToDisplay(const std::string& ip, unsigned short port);
    bool isValidData(const std::string& input);
    std::string processData(const std::string& input);
};
