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

void parse_read(Connection_State &state) {

    //Populate state.outcoming here
    state.want_write = true;
}

void handle_write(Connection_State &state) {
    //Write contents of outgoing to file descriptor here.
    size_t msg_len = state.outgoing.size();

    int bytes_written = 0;
    bytes_written = send(state.fd, &msg_len, 4, 0);

    if(bytes_written == -1) {
        state.want_close = true;
        return;
    }

    bytes_written = send(state.fd, state.outgoing.data(), msg_len, 0);

    if(bytes_written == -1) {
        state.want_close = true;
        return;
    }

    state.want_close = true;
    state.want_write = false;
}

void process_request(Connection_State &state) {
    //TODO: Change this to actual callback function.
    char response[] = "Server received the request!";
    for(int i = 0; i < strlen(response); ++i) {
        state.outgoing.push_back(response[i]);
    }
}

void handle_read(Connection_State &state) {
    char buf;
    while(state.incoming.size() < 4) {
        if((read(state.fd, &buf, 1) == -1)) {
            return;
        }
        else {
            state.incoming.push_back(buf);
        }
    }
    
    size_t incoming_length;
    
    memcpy(&incoming_length, state.incoming.data(), 4);
    
    while(state.incoming.size() < 4 + incoming_length) {
        if((read(state.fd, &buf, 1) == -1) && 
            (errno == EAGAIN || errno == EWOULDBLOCK)) {
            return;
        }
        else {
            state.incoming.push_back(buf);
        }
    }
    std::cout << "Server received message:\n";
    for(int i = 0; i < incoming_length; ++i) {
        std::cout << (char) state.incoming[4 + i];
    }
    std::cout << std::endl;
    state.want_read = false;
    process_request(state);
    handle_write(state);
}

void add_connection(int incoming_fd, std::vector<pollfd> &connections, std::vector<Connection_State> &states) {
    
    pollfd pfd;
    pfd.fd = incoming_fd;
    pfd.events = 0;
    pfd.revents = 0;


    Connection_State state;
    state.fd = incoming_fd;
    state.want_read = true;

    connections.push_back(pfd);
    states.push_back(state);
}

void delete_connection(int idx, std::vector<pollfd> &connections, std::vector<Connection_State> &states) {

    connections[idx] = connections[connections.size() - 1];
    states[idx] = states[states.size() - 1];

    connections.pop_back();
    states.pop_back();

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

    add_connection(fd, connections, states);
    

    while(true) {

        for(int i = 0; i < connections.size(); ++i) {
            connections[i].events = 0;
            if(states[i].want_read) {
                connections[i].events |= POLLIN;
            }
            if(states[i].want_write) {
                connections[i].events |= POLLOUT;
            }
            if(states[i].want_close) {
                connections[i].events |= POLLHUP;
            }
        }

        int poll_updates = poll(connections.data(), connections.size(), -1);

        for(int i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLIN) {

                if(connections[i].fd == fd) {
                    int incoming_fd = accept(fd, (sockaddr*) &client, &addrlen);
                    add_connection(incoming_fd, connections, states);
                    connections[i].revents = 0;
                }
                else {
                    //Insert read callback here
                    handle_read(states[i]);
                }
            }
            if(connections[i].revents & POLLOUT) {
                // Insert write callback here
                handle_write(states[i]);
            }
            if(connections[i].revents & POLLHUP || states[i].want_close) {
                // Insert close callback here
                int socket_fd = connections[i].fd;
                delete_connection(i, connections, states);
                close(socket_fd);
                std::cout << "Server closed connection " << socket_fd << std::endl;
            }
        }

    }
}

int main() {

    int fd = init_server();

    run_server(fd);

    return 0;
}