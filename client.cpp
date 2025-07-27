#include "msg_lib.hpp"
#include <iostream>

int main() {
    set_request("Hello", "Adele");
    
    char response_buffer[MAX_MSG_LEN];
    int response_len;

    get_request("Hello", response_buffer, response_len);
    for(int i = 0; i < response_len; ++i) {
        std::cout <<response_buffer[i];
    }
    std::cout << std::endl;

    return 0;
}