#include "msg_lib.h"
#include <iostream>

struct Connection_State {

    int fd = -1;
    
    bool want_read = false;
    bool want_write = false;
    bool want_close = false;

    std::vector<uint8_t> incoming;
    std::vector<uint8_t> outgoing;
};

int req_resp_handler(int connection_fd) {
    char msg[4 + MAX_MSG_LEN];

    int error = recv_all(connection_fd, msg, 4);

    if(error < 0) {
        return error;
    }

    uint32_t len = 0;
    memcpy(&len, msg, 4);

    if(len >= MAX_MSG_LEN) {
        return -1;
    }

    error = recv_all(connection_fd, msg + 4, len);
    
    if(error < 0) {
        return error;
    }

    for(int i = 0; i < len; ++i) {
        std::cout << msg[4 + i];
    }
    std::cout << std::endl;

    char server_msg[] = "bye from the other side";
    size_t str_len = strlen(server_msg);

    send_all(connection_fd, (char*)&str_len, 4);
    send_all(connection_fd, server_msg, str_len);
    return 0;

}

void connection_handler(char* incoming, char* outgoing) {

}

void add_connection() {

}

void delete_connection() {

}

int init_server() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    assert(fd >= 0);

    int val = 1;
    int option = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val));

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);


    int network_status = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    return fd;
}

void run_server(int fd) {
    listen(fd, MAX_MSG_LEN);

    struct sockaddr_in client{};
    socklen_t addrlen = sizeof(client);



    std::vector<pollfd> connections;
    std::vector<Connection_State> states;
    pollfd std_input;
    std_input.fd = 0;
    std_input.events = POLLIN;
    connections.push_back(std_input);
    
    while(true) {
        int fd = accept(fd, reinterpret_cast<sockaddr*>(&client), &addrlen);
        if(fd > -1) {
            //Add another socket descriptor to our polling list
            std::cout <<"We have accepted a connection and should create a file descriptor" << std::endl;
        }
        //Poll the list here
        int poll_updates = poll(connections.data(), connections.size(), -1);

        //Handle reads here
        for(int i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLIN) {
                // Handle read here
                std::cout << "We found a reading connection" << std::endl;
            }
        }
        //Handle writes here
        for(int i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLOUT) {
                // Handle write here
            }
        }
        //Handle closes here
        for(int i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLHUP) {
                // Handle close here
            }
        }
    }
}

int main() {

    int fd = init_server();

    run_server(fd);

    return 0;
}