#pragma once
#include "common.h"
#include <winsock2.h>

class Client {
public:
    Client(const std::string& ip, int port);
    ~Client();
    void run();

private:
    SOCKET client_socket;
    struct sockaddr_in server_address;
};
