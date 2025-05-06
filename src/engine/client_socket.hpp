#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

// might wanna put these includes in the cpp files instead of in the hpp files
#ifdef __linux__
// linux specific includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef _WIN32
// windows specific includes
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CLIENT_DISCONNECT_MSG "Other client disconnected"
#define SERVER_DISCONNECT_MSG "Server disconnected"

enum homebrew_error {
    CLIENT_DISC_ERRNO = -1,
    SERVER_DISC_ERRNO = -2,
};

const char* get_error_message(int error_code);

// ClientSocket class definition
class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();
    
    void connect_to(const char* server_ip, int port);
    void send_msg(std::string msg);
    void send_msg(const char* msg, int msg_size);
    size_t receive_msg(char* buffer, int buffer_size);
    void close_socket();

private:
    int clientSocket;
    struct sockaddr_in serverAddress;
};

#endif
