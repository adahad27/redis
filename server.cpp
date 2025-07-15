#include "msg_lib.h"

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

    printf("Client said: %*s\n", len, msg + 4);
    return 0;

}

int main() {

    int fd = socket(AF_INET6, SOCK_STREAM, 0);
    
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    uint32_t port = 8000;

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);


    int network_status = bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    //Add an assert that checks network status here.
    assert(network_status);

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