#include "client.h"
#include <iostream>
#include <ws2tcpip.h> // Для InetPtonA
#include <string>

Client::Client(const std::string& ip, int port) {
    // Ініціалізація Winsock
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // Використання версії для ASCII: InetPtonA
    if (InetPtonA(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }
}

Client::~Client() {
    closesocket(client_socket);
    WSACleanup();
}

void Client::run() {
    std::string message = "Hello from client!";
    send(client_socket, message.c_str(), message.size(), 0);

    char buffer[1024] = {0};
    recv(client_socket, buffer, 1024, 0);

    std::cout << "Client received: " << buffer << std::endl;
}
