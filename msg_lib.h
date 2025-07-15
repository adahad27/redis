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



int recv_all(int connfd, char* buf, int n);

int send_all(int connfd, char* buf, int n);





#endif