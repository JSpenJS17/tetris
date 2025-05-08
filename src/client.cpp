#include "client.hpp"

// externs
ClientSocket* client_socket = nullptr;
atomic<bool> stop_flag(false);
atomic<bool> received_anything(false);
mutex game_mutex;
mutex other_game_mutex;
string starting_string = "Hello from client";

int check_error(string wherefrom)
{
    if (errno) {
        fflush(stdout);
        reset_everything();
        clear_screen();
        cout << "Crash! From " << wherefrom << endl;
        cout << "Error Number: " << errno << endl;
        cout << "Error: " << strerror(errno) << endl;
        exit(errno);
    }
    return 0;
}

int row = 0;

void sender() {
    client_socket->send_msg(starting_string);
    while (!received_anything.load()) {
        delay(100);
    }

    // wait for the listener to receive the first message
    while (!stop_flag.load()) {
        vector<vector<Pixel>> our_board;

        {
            lock_guard<mutex> lock(game_mutex);
            our_board = game.get_board();  // Make a copy while locked
        }

        std::vector<char> buffer;

        // First: encode number of rows and columns (assumes uniform row sizes)
        uint32_t rows = our_board.size();
        uint32_t cols = rows ? our_board[0].size() : 0;

        buffer.insert(buffer.end(), (char*)&rows, (char*)&rows + sizeof(rows));
        buffer.insert(buffer.end(), (char*)&cols, (char*)&cols + sizeof(cols));

        // Now serialize each Pixel
        for (const auto& row : our_board) {
            for (const Pixel& pixel : row) {
                buffer.push_back(pixel.bgc);
                buffer.push_back(pixel.fgc);
                buffer.push_back(pixel.val);
            }
        }

        client_socket->send_msg(buffer.data(), buffer.size());
        check_error("sender()");
        delay(250);
    }
}


void listener() {
    char receive_buffer[BUFFER_SIZE];
    size_t wait_time = 0;

    // wait for the sender to send the first message
    while (!received_anything.load()) {
        size_t bytes_received = client_socket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error("listener()");

        if (bytes_received > 0) {
            received_anything.store(true);
        }
    }

    while (!stop_flag.load()) {
        size_t bytes_received = client_socket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error("listener()");

        size_t offset = 0;
        uint32_t rows, cols;

        if (bytes_received < sizeof(rows) + sizeof(cols)) continue; // not enough data

        memcpy(&rows, receive_buffer + offset, sizeof(rows)); offset += sizeof(rows);
        memcpy(&cols, receive_buffer + offset, sizeof(cols)); offset += sizeof(cols);

        vector<vector<Pixel>> board = {};
        for (uint32_t i = 0; i < rows; ++i) {
            board.push_back(vector<Pixel>(cols));
        }

        for (uint32_t i = 0; i < rows; ++i) {
            for (uint32_t j = 0; j < cols; ++j) {
                if (offset + 3 > bytes_received) break;

                char bgc, fgc, val;

                bgc = receive_buffer[offset++];
                fgc = receive_buffer[offset++];
                val = receive_buffer[offset++];

                board[i][j] = Pixel(val, bgc, fgc);
            }
        }

        {
            lock_guard<mutex> lock(other_game_mutex);
            other_game.set_board(board);
        }
    }
}


void connect_to_server() {
    // specifying address
    const char* server_ip = "131.186.7.78";
    int port = 5000;

    if (!client_socket) {
        client_socket = new ClientSocket();
    }

    client_socket->connect_to(server_ip, port);
    check_error("connect_to_server() -- connect_to");
}
