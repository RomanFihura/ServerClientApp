#pragma once
#include "common.h"
#include <winsock2.h> //use Windows Sockets API (Winsock) 
#include <vector>
#include <thread>
#include <mutex>

class Server {
public:
    Server(int port);
    ~Server();
    void run(); //func for serv setup

private:
    SOCKET server_fd; //for serv socket
    struct sockaddr_in address; //struct for information about serv address
    int addrlen; //for bind and accept
    std::vector<std::thread> client_threads; 
    std::mutex client_threads_mutex;

    void handleClient(SOCKET client_socket); //func for handling client requests
};
