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
#include <fcntl.h>
#include <stdbool.h>
#include <string>

const uint32_t port = 8080;
const uint32_t MAX_MSG_LEN = 1000;

// int recv_all(int connfd, char* buf, int n);

// int send_all(int connfd, char* buf, int n);

void get_request(std::string key, char* response, int &response_len);

void del_request(std::string key, char* response, int& response_len);

void set_request(std::string key, std::string value, char* response, int& response_len);

void print_resp(char* response, int response_len);

#endif