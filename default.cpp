#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 12345;
const std::string RESPONSE = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, World!\n";

void handle_client(int client_socket) {
    char buffer[4096];
    ssize_t bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        std::cout << "Request:\n" << buffer << std::endl;

        // Send the HTTP response
        write(client_socket, RESPONSE.c_str(), RESPONSE.size());
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket\n";
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error listening on socket\n";
        return 1;
    }

    std::cout << "Server is running on port " << PORT << "\n";

    while (true) {
        sockaddr_in client_addr = {};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);

        if (client_socket < 0) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Client connected\n";
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}
