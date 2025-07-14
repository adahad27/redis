#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cassert>
#include <unistd.h>
#include <string.h>

const uint32_t MAX_MSG_LEN = 1000; 

int recv_all(int connfd, char* buf, int n) {
    int read_bytes = 0;
    int result = 0;
    while(read_bytes < n) {
        result = recv(connfd, buf, n, 0);
        if(result < 0) {
            return -1;
        }
    }
    return 0;
}

int send_all(int connfd, char* buf, int n) {
    int write_bytes = 0;
    int result = 0;
    while(write_bytes < n) {
        result = send(connfd, buf, n, 0);
        if(result < 0) {
            return -1;
        }
    }
    return 0;
}

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



}

int main() {

    int fd = socket(AF_INET6, SOCK_STREAM, 0);
    
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    uint32_t port = 80;

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
        close(connection_fd);
    }
}