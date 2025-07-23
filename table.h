#ifndef TBL
#define TBL
#include "msg_lib.h"
#include <string>
#include <memory>

struct Node {
    Node *next;
    u_long hash = 0;
};

struct Datum {
    std::string value;
    Node node;
};

Datum* get_container(Node *node);

class HTable {
    private:
        Node **table;
        uint32_t size;
        u_long hash(const char *key, uint32_t size);
    
    public:
        HTable(uint32_t size);
        void insert(const std::string key, const std::string value);
        std::string get(const std::string key);
        void remove(const std::string key);
        ~HTable();


};





#endif