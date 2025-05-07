#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "game.hpp"
#include <atomic>

using namespace std;
#define BUFFER_SIZE 1024

extern ClientSocket* client_socket;
extern atomic<bool> stop_flag;
int check_error(string wherefrom);
void sender();
void listener();
void connect_to_server();
void start_client_threads();

#endif