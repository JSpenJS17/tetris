#include "client.hpp"

string starting_string = "Hello from client";
ClientSocket* clientSocket = new ClientSocket();

int check_error(string wherefrom)
{
    if (errno) {
        fflush(stdout);
        cout << "Crash! From " << wherefrom << endl;
        cout << "Error Number: " << errno << endl;
        cout << "Error: " << strerror(errno) << endl;
        return 1;
    }
    return 0;
}

int row = 0;

void sender() {
    clientSocket->send_msg(starting_string);

    while (true) {
        vector<PosPixel>* changes = game.get_changes();

        size_t size = changes->size();

        // Serialize
        std::vector<char> buffer;

        // First: number of PosPixels
        buffer.push_back(static_cast<char>(size)); // 1 byte

        for (const PosPixel& p : *changes) {
            // Serialize row and col
            buffer.insert(buffer.end(), (char*)&p.row, (char*)&p.row + sizeof(p.row));
            buffer.insert(buffer.end(), (char*)&p.col, (char*)&p.col + sizeof(p.col));

            // Serialize face
            buffer.insert(buffer.end(), (char*)&p.face.bgc, (char*)&p.face.bgc + sizeof(p.face.bgc));
            buffer.insert(buffer.end(), (char*)&p.face.fgc, (char*)&p.face.fgc + sizeof(p.face.fgc));
            buffer.push_back(p.face.val);
        }

        clientSocket->send_msg(buffer.data(), buffer.size());
        check_error("sender()");
        delay(16);
    }
}

void listener() {
    char receive_buffer[BUFFER_SIZE];
    size_t bytes_received;
    size_t wait_time = 0;

    while (true) {
        bytes_received = clientSocket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error("listener()");

        if (wait_time < starting_string.size()) {
            wait_time += bytes_received;
            continue;
        }

        // First byte = number of PosPixels
        uint8_t num_pixels = receive_buffer[0];

        vector<PosPixel> received;
        size_t offset = 1;

        for (int i = 0; i < num_pixels; ++i) {
            if (offset + 13 > bytes_received) break; // error check: 13 bytes per PosPixel

            unsigned int row, col;
            unsigned short bgc, fgc;
            char val;

            memcpy(&row, receive_buffer + offset, sizeof(row)); offset += sizeof(row);
            memcpy(&col, receive_buffer + offset, sizeof(col)); offset += sizeof(col);
            memcpy(&bgc, receive_buffer + offset, sizeof(bgc)); offset += sizeof(bgc);
            memcpy(&fgc, receive_buffer + offset, sizeof(fgc)); offset += sizeof(fgc);
            val = receive_buffer[offset++];

            received.emplace_back(row, col, Pixel(val, bgc, fgc));
        }

        // Now `received` has all the PosPixels
        other_game.set_changes(&received);
    }
}
