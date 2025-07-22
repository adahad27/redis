#include "table.h"

const unsigned long FNV_BASIS = 14695981039346656037;
const unsigned long FNV_OFFSET = 1099511628211;


u_long HTable::hash(const char *key, uint32_t size) {
    u_long current_hash = FNV_BASIS;

    for(uint32_t i = 0; i < size; ++i) {
        current_hash = current_hash ^ key[i];
        current_hash *= FNV_OFFSET;
    }

    return current_hash;

}

HTable::HTable(uint32_t size) {
    this->table = (Node**) calloc(sizeof(Node*), size);
    this->size = 0;
}

void HTable::insert(std::string key, const std::string value) {
    this->size += 1;
    //TODO: Insert check for growing logic here

    /* 
    1.) Calculate hash for key
    2.) Calculate position of value
    3.) Linked list insertion
    */

    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val % this->size;

}



