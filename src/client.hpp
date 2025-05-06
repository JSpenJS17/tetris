#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "game.hpp"
#include "engine/client_socket.hpp"
#define BUFFER_SIZE 1024

extern ClientSocket* clientSocket;
int check_error(string wherefrom);
void sender();
void listener();

#endif