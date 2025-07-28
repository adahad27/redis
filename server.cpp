#include "msg_lib.hpp"
#include "table.hpp"
#include <iostream>

HMap map(1);


struct Connection_State {

    int fd = -1;
    
    bool want_read = false;
    bool want_write = false;
    bool want_close = false;

    std::vector<uint8_t> incoming;
    std::vector<uint8_t> outgoing;
};



void handle_write(Connection_State &state) {
    //Write contents of outgoing to file descriptor here.
    size_t msg_len = state.outgoing.size();

    if(send(state.fd, &msg_len, 4, MSG_NOSIGNAL) <= 0) {
        state.want_close = true;
        state.want_write = false;
        return;
    }

    if(send(state.fd, state.outgoing.data(), msg_len, MSG_NOSIGNAL) <= 0) {
        state.want_close = true;
        state.want_write = false;
        return;
    }

    state.want_close = true;
    state.want_write = false;
}

void process_request(Connection_State &state) {
    
    int num_cmds;
    int arg_len;
    char cmd[4];

    std::vector<std::string> arguments;

    memcpy(&num_cmds, state.incoming.data(), 4);
    arguments.resize(num_cmds - 1);

    memcpy(cmd, state.incoming.data() + 4, 3);
    cmd[3] = 0;
    uint32_t buffer_end = 7;

    for(int i = 0; i < num_cmds - 1; ++i) {
        memcpy(&arg_len, state.incoming.data() + buffer_end, 4);
        buffer_end += 4;
        for(int j = 0; j < arg_len; ++j) {
            arguments[i].push_back(state.incoming[buffer_end + j]);
        }
        buffer_end += arg_len;

    }

    if(!strcmp(cmd, "set")) {
        map.insert(arguments[0], arguments[1]);
        char response[] = "Key-Value pair inserted";
        for(uint32_t i = 0; i < strlen(response); ++i) {
            state.outgoing.push_back(response[i]);
        }
    }
    else if(!strcmp(cmd, "get")) {
        std::string response = map.get(arguments[0]);
        for(uint32_t i = 0; i < response.size(); ++i) {
            state.outgoing.push_back((char)response[i]);
        }
    }
    else if(!strcmp(cmd, "del")) {
        map.remove(arguments[0]);
        char response[] = "Key-Value pair deleted";
        for(uint32_t i = 0; i < strlen(response); ++i) {
            state.outgoing.push_back(response[i]);
        }
    }
}

int handle_read(Connection_State &state) {

    char num_cmds[4];

    if(recv(state.fd, num_cmds, 4, 0) <= 0) {
        state.want_close = true;
        return -1;
    }

    uint32_t cmds, len;
    memcpy(&cmds, num_cmds, 4);

    for(uint32_t i = 0; i < cmds; ++i) {

        uint32_t buffer_end = state.incoming.size();
        state.incoming.resize(buffer_end + 4);

        if(recv(state.fd, state.incoming.data() + buffer_end, 4, 0) <= 0) {
            state.want_close = true;
            return -1;
        }

        memcpy(&len, state.incoming.data() + buffer_end, 4);
        buffer_end = state.incoming.size();
        state.incoming.resize(buffer_end + len);
        
        if(recv(state.fd, state.incoming.data() + buffer_end, len, 0) <= 0) {
            state.want_close = true;
            return -1;
        }

    }
    
    state.want_read = false;
    process_request(state);
    handle_write(state);

    return 0;
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
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val));

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);


    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
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

        for(uint32_t i = 0; i < connections.size(); ++i) {
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

        poll(connections.data(), connections.size(), -1);

        for(uint32_t i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLIN) {

                if(connections[i].fd == fd) {
                    int incoming_fd = accept(fd, (sockaddr*) &client, &addrlen);
                    add_connection(incoming_fd, connections, states);
                    connections[i].revents = 0;
                    std::cout << "Connection established on socket: " << incoming_fd << std::endl;
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
                std::cout << "Connection closed on socket: " << socket_fd << std::endl;
            }
        }

    }
}

int main() {

    int fd = init_server();

    run_server(fd);

    return 0;
}