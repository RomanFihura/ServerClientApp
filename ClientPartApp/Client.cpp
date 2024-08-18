#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while (true) {
        int valread = recv(sock, buffer, 1024, 0);
        if (valread <= 0) break; // Перевірка на помилки при отриманні даних
        buffer[valread] = '\0';
        std::cout << buffer;

        std::string input;
        std::getline(std::cin, input);
        send(sock, input.c_str(), input.size(), 0);

        valread = recv(sock, buffer, 1024, 0);
        if (valread <= 0) break;
        buffer[valread] = '\0';
        std::cout << "Server response: " << buffer << std::endl;

        std::string input2;
        std::getline(std::cin, input2);
        send(sock, input2.c_str(), input2.size(), 0);
    }

    closesocket(sock);
    WSACleanup();
    std::cout << "You ended your work with server";
    return 0;
}
