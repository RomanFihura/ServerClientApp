#include "server.h"
#include <iostream>

Server::Server(int port) : addrlen(sizeof(address)) {
    // init Winsock
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
}

Server::~Server() {
    closesocket(server_fd);
    WSACleanup();

    // wainting for end of all clients threads
    for (std::thread &t : client_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void Server::run() {
    std::cout << "Server is running and waiting for clients..." << std::endl;
    while (true) {
        SOCKET client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket != INVALID_SOCKET) {
            std::cout << "New client connected." << std::endl;

            // exec new thread for client
            std::lock_guard<std::mutex> lock(client_threads_mutex);
            client_threads.emplace_back(&Server::handleClient, this, client_socket);
        }
    }
}

void Server::handleClient(SOCKET client_socket) {
    char buffer[1024] = {0};
    int valread = recv(client_socket, buffer, 1024, 0);
    if (valread > 0) {
        std::string request(buffer);

        // handling client request
        std::string response = "Server received: " + request;
        send(client_socket, response.c_str(), response.size(), 0);
    }

    std::cout << "Client disconnected." << std::endl;
    closesocket(client_socket);
}
