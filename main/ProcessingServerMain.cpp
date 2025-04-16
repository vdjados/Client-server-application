#include "net/ProcessingServer.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <listen_port> <display_ip> <display_port>\n";
        return 1;
    }

    unsigned short listenPort  = static_cast<unsigned short>(std::stoi(argv[1]));
    std::string displayIp      = argv[2];
    unsigned short displayPort = static_cast<unsigned short>(std::stoi(argv[3]));

    ProcessingServer server;
    server.run(listenPort, displayIp, displayPort);
    return 0;
}
