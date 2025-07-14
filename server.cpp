#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cassert>
#include <unistd.h>

const uint32_t MAX_MSG_LEN = 1000; 

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