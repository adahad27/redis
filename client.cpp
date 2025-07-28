#include "msg_lib.hpp"

int main() {
    
    char response_buffer[MAX_MSG_LEN];
    int response_len;

    set_request("Hello", "Adele", response_buffer, response_len);
    print_resp(response_buffer, response_len);
    
    
    set_request("Summer Days", "Macklemore", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("All The Stars", "Kendrick Lamar", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("Stay", "Zedd", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    del_request("Hello", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("Maps", "Maroon 5", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    get_request("Summer Days", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("Gone are the Days", "Kygo", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("These are the Lies", "The Cab", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("Duality", "Set it Off", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    get_request("Duality", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    set_request("Whisper", "Burn the Ballroom", response_buffer, response_len);
    print_resp(response_buffer, response_len);

    return 0;
}