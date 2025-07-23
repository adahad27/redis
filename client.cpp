#include "msg_lib.hpp"
#include <iostream>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd);
    

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    int err_status = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    assert(err_status >= 0);
    
    char* len;
    char msg[] = "hello from the other side";
    char msg_2[] = "side other the from hello";
    size_t str_len = strlen(msg);
    size_t num_cmds = 2;
    
    send_all(fd, (char*)&num_cmds, 4);

    send_all(fd, (char*)&str_len, 4);
    send_all(fd, msg, strlen(msg));

    send_all(fd, (char*)&str_len, 4);
    send_all(fd, msg_2, strlen(msg_2));

    char response[4 + MAX_MSG_LEN];

    recv_all(fd, response, 4);
    memcpy(&str_len, response, 4);
    recv_all(fd, response + 4, str_len);

    std::cout << "Response was:\n";
    for(int i = 0; i < str_len; ++i) {
        std::cout << response[4 + i];
    }
    std::cout<<"\nResponse Finished.\n";


    close(fd);

    return 0;
}