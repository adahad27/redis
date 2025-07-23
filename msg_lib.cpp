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

