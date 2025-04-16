#pragma once

#include "ServerBase.h"

class DisplayServer : public ServerBase {
protected:
    void handleClient(SocketWrapper& clientSocket) override;
};
