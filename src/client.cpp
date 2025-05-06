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

void sender() {
    clientSocket->send_msg(starting_string);
    while (true) {
        // Send it!
        clientSocket->send_msg((char*)&game, 1);
        check_error("sender()");

        delay(100); // 10 hz approximately
    }
}

void listener() {
    char receive_buffer[BUFFER_SIZE];
    size_t bytes_received;
    size_t wait_time = 0;
    
    while (true) {
        bytes_received = clientSocket->receive_msg(receive_buffer, sizeof(receive_buffer));
        check_error("listener()");
        
        if (wait_time < starting_string.size())
        {
            // wait through first "Hello from client"
                // this prevents the dreaded 35 character crash
            wait_time += bytes_received;
            continue;
        }

        
    }
}

