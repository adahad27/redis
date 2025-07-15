#include "msg_lib.h"
#include <iostream>

const uint32_t MAX_MSG_LEN = 1000; 


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
    std::cout << "Client said: ";

    for(int i = 0; i < len; ++i) {
        std::cout << msg[4 + i];
    }
    std::cout << std::endl;
    
    return 0;

}

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);

    int val = 1;
    int option = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val));
    assert(option >= 0);

    uint32_t port = 8080;

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);


    int network_status = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    //Add an assert that checks network status here.
    assert(network_status >= 0);

    listen(fd, MAX_MSG_LEN);

    struct sockaddr_in client{};
    socklen_t addrlen = sizeof(client);
    while(true) {
        int connection_fd = accept(fd, reinterpret_cast<sockaddr *>(&client), &addrlen);
        assert(connection_fd);
        //Redis work
        req_resp_handler(connection_fd);
        close(connection_fd);
    }

    return 0;
}