#ifndef MSGLIB
#define MSGLIB

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cassert>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <vector>

const uint32_t port = 8080;
const uint32_t MAX_MSG_LEN = 1000;

int recv_all(int connfd, char* buf, int n);

int send_all(int connfd, char* buf, int n);





#endif