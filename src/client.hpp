#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "game.hpp"

using namespace std;
#define BUFFER_SIZE 1024

extern mutex game_mutex;
extern mutex other_game_mutex;
extern ClientSocket* client_socket;
extern atomic<bool> stop_flag;
extern atomic<bool> received_anything;
int check_error(string wherefrom);
void sender();
void listener();
void connect_to_server();
void start_client_threads();

#endif