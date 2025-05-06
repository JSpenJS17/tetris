#include "client_socket.hpp"

#ifdef _WIN32
// ClientSocket class implementation
ClientSocket::ClientSocket() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == (int)INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

ClientSocket::~ClientSocket() {
    // Cleanup resources
    close_socket();
}

void ClientSocket::connect_to(const char* server_ip, int port) {
    // Connect to server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

void ClientSocket::send_msg(std::string msg) {
    // Send data (std::string) to server
    send_msg(msg.c_str(), static_cast<int>(msg.size()));
}

void ClientSocket::send_msg(const char* msg, int msg_size) {
    // Send data (null-terminated string) to server
    if (send(clientSocket, msg, msg_size, 0) == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

size_t ClientSocket::receive_msg(char* buffer, int buffer_size) {
    // Receive data from server
    memset(buffer, 0, buffer_size);
    int bytesReceived = recv(clientSocket, buffer, buffer_size, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
    }
    return bytesReceived;
}

void ClientSocket::close_socket() {
    // Close connection without killing the socket object
    if (clientSocket != (int)INVALID_SOCKET) {
        closesocket(clientSocket);
        clientSocket = (int)INVALID_SOCKET;
    }
    WSACleanup(); // Cleanup Winsock
}



#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
// ClientSocket class implementation
ClientSocket::  ClientSocket() {
    // Constructor
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
}

ClientSocket::~ClientSocket() {
    // Destructor
    close_socket();
}

void ClientSocket::connect_to(const char* server_ip, int port) {
    // takes server IP like so: "xxx.xxx.xxx.xxx"
    // port is just an integer
    // Connect to server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(server_ip);
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void ClientSocket::send_msg(std::string msg) {
    // Send data (std::string) to server
    send_msg(msg.c_str(), msg.size());    
}

void ClientSocket::send_msg(const char* msg, int msg_size) {
    // Send data (null terminated string) to server
    send(clientSocket, msg, msg_size, 0);
}

size_t ClientSocket::receive_msg(char* buffer, int buffer_size) {
    // Receive data from server
    memset(buffer, 0, buffer_size);
    // do not block
    return recv(clientSocket, buffer, buffer_size, 0);
}

void ClientSocket::close_socket() {
    // Close connection without killing the socket object
    close(clientSocket);
}

#endif

const char* get_error_message(int error_code) {
    // Get error message based on error code
    switch (error_code) {
        case CLIENT_DISC_ERRNO:
            return CLIENT_DISCONNECT_MSG;
        case SERVER_DISC_ERRNO:
            return SERVER_DISCONNECT_MSG;
        default:
            return "Unknown error";
    }
}