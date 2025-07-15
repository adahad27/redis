#include "msg_lib.h"
#include <iostream>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd);
    
    uint32_t port = 8080;

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    int err_status = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    assert(err_status >= 0);
    
    char* len;
    char msg[] = "hello from the other side";
    size_t str_len = strlen(msg);
    

    send_all(fd, (char*)&str_len, 4);
    send_all(fd, msg, strlen(msg));

    close(fd);

    return 0;
}