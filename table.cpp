#include "table.h"


struct Node {
    Node *next;
    uint64_t hash = 0;
};

struct htable {
    Node **table;
    uint32_t size;
    
};


