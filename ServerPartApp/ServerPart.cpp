#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <thread>
#include <algorithm>
#include <sstream>
#include <limits>


#pragma comment(lib, "Ws2_32.lib")

bool isPalindrome(const std::string& str) {
    std::string reversed_str = str;
    std::reverse(reversed_str.begin(), reversed_str.end());
    return str == reversed_str;
}

bool hasDigit(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isdigit);
}

int sumOfThreeNumbers(const std::string& str) {
    int a, b, c;
    sscanf_s(str.c_str(), "%d %d %d", &a, &b, &c);
    return a + b + c;
}
void findMin(const std::vector<int>& numbers, int& min_value) {
    min_value = *std::min_element(numbers.begin(), numbers.end());
}

void findMax(const std::vector<int>& numbers, int& max_value) {
    max_value = *std::max_element(numbers.begin(), numbers.end());
}

std::pair<int, int> findMinMax(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return { 0,0 };
    }
    int min_value = numbers[0];
    int max_value = numbers[0];

    std::thread t1(findMin, std::cref(numbers), std::ref(min_value));
    std::thread t2(findMax, std::cref(numbers), std::ref(max_value));

    t1.join();
    t2.join();

    return { min_value, max_value };
}

std::vector<int> parseNumbers(const std::string& str) {
    std::vector<int> numbers;
    std::stringstream ss(str);
    int number;
    while (ss >> number) {
        numbers.push_back(number);
    }
    return numbers;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    std::string message;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3); //listen(server_fd, 3);

    std::cout << "Server is waiting for a connection..." << std::endl;

    client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

    while (true) {
        std::string response;
        message = "Choose an option:\n1. Check palindrome\n2. Check if the message contains digits\n3. Sum three numbers\n4. Find min and max in ur array\n0. End connection\n";
        send(client_socket, message.c_str(), message.size(), 0);

        int valread = recv(client_socket, buffer, 1024, 0);
        if (valread <= 0) break; 
        buffer[valread] = '\0';
        std::string option(buffer);
        try {
            int choice = std::stoi(option);

            switch (choice) {
            case 0: {
                std::cout << "Cliend finished his work" << std::endl;
                closesocket(client_socket);
                closesocket(server_fd);
                WSACleanup();
                return 0;
            }
            case 1: {
                message = "Send a number to check if it's a palindrome: ";
                send(client_socket, message.c_str(), message.size(), 0);
                valread = recv(client_socket, buffer, 1024, 0);
                if (valread <= 0) break;
                buffer[valread] = '\0';
                std::string number(buffer);

                response = isPalindrome(number) ? "Yes\n" : "No\n";
                break;
            }
            case 2: {
                message = "Send a message to check for digits: ";
                send(client_socket, message.c_str(), message.size(), 0);
                valread = recv(client_socket, buffer, 1024, 0);
                if (valread <= 0) break;
                buffer[valread] = '\0';
                std::string text(buffer);

                response = hasDigit(text) ? "Yes\n" : "No\n";
                break;
            }
            case 3: {
                message = "Send three numbers separated by spaces: ";
                send(client_socket, message.c_str(), message.size(), 0);
                valread = recv(client_socket, buffer, 1024, 0);
                if (valread <= 0) break;
                buffer[valread] = '\0';
                std::string numbers(buffer);

                int sum = sumOfThreeNumbers(numbers);
                response = "Sum: " + std::to_string(sum) + "\n";
                break;
            }
            case 4: {
                message = "Send an array of numbers separated by spaces: ";
                send(client_socket, message.c_str(), message.size(), 0);
                valread = recv(client_socket, buffer, 1024, 0);
                if (valread <= 0) break;
                buffer[valread] = '\0';
                std::string numbers_str(buffer);

                std::vector<int> numbers = parseNumbers(numbers_str);
                auto result = findMinMax(numbers);
                response = "Min: " + std::to_string(result.first) + ", Max: " + std::to_string(result.second) + "\n";
                break;
            }
            default: {
                response = "Invalid option. Please try again.";
                break;
            }
            }
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid argument: " << e.what() << std::endl;
        }

        send(client_socket, response.c_str(), response.size(), 0);
    }

    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
