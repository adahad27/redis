#include "msg_lib.hpp"


int recv_all(int connfd, char* buf, int n) {
    int read_bytes = 0;
    int result = 0;
    while(read_bytes < n) {
        result = recv(connfd, buf, n, 0);
        if(result < 0) {
            return -1;
        }
        read_bytes += result;
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
        write_bytes += result;
    }
    return 0;
}

int establish_connection() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd);
    

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    int err_status = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    assert(err_status >= 0);

    return fd;
}

void get_request(std::string key, char* response, int &response_len) {
    
    int fd = establish_connection();

    char command[] = "get";
    char *arg = key.data();

    int command_len = strlen(command);
    int arg_len = strlen(arg);

    int num_cmd = 2;
    send_all(fd, (char*)&num_cmd, 4);


    send_all(fd, (char *) &command_len, 4);
    send_all(fd, command, command_len);

    send_all(fd, (char*) &arg_len, 4);
    send_all(fd, arg, arg_len);

    recv_all(fd, (char*) &response_len, 4);
    recv_all(fd, response, response_len);


}

void del_request(std::string key) {
    int fd = establish_connection();

    char command[] = "del";
    char *arg = key.data();

    int command_len = strlen(command);
    int arg_len = strlen(arg);

    int num_cmd = 2;
    send_all(fd, (char*)&num_cmd, 4);

    send_all(fd, (char *) &command_len, 4);
    send_all(fd, command, command_len);

    send_all(fd, (char*) &arg_len, 4);
    send_all(fd, arg, arg_len);
}

void set_request(std::string key, std::string value) {
    int fd = establish_connection();

    char command[] = "set";
    char *arg_1 = key.data();
    char *arg_2 = value.data();

    int command_len = strlen(command);
    int arg_1_len = strlen(arg_1);
    int arg_2_len = strlen(arg_2);

    int num_cmd = 3;
    send_all(fd, (char*)&num_cmd, 4);

    send_all(fd, (char *) &command_len, 4);
    send_all(fd, command, command_len);

    send_all(fd, (char*) &arg_1_len, 4);
    send_all(fd, arg_1, arg_1_len);

    send_all(fd, (char*) &arg_2_len, 4);
    send_all(fd, arg_2, arg_2_len);
}