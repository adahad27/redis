#include "table.hpp"
#include <iostream>
#define DEBUG(x) std::cout<<"Reached line " << x << std::endl;
#define FNV_BASIS 14695981039346656037
#define FNV_OFFSET 1099511628211


Datum* get_container(Node *node_ptr) {
    return (Datum*)((char*)node_ptr - offsetof(Datum, node));
}

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

void HTable::insert(const std::string key, const std::string value) {
    //TODO: Insert check for growing logic here

    /* 
    1.) Calculate hash for key
    2.) Calculate position of value
    3.) Linked list insertion
    */

    u_long hash_val = hash(key.data(), key.size());
    long bucket = fmod(hash_val, size);

    //TODO: Add check for modifying already existing keys in the table

    Datum *data = new Datum{value, {nullptr, hash_val}};
    Node *current = this->table[bucket];
    //Insertion occurs at the front
    data->node.next = this->table[bucket];
    this->table[bucket] = &data->node;
    

}

std::string HTable::get(const std::string key) {
    u_long hash_val = hash(key.data(), key.size());
    long bucket = fmod(hash_val, size);

    Node *current = this->table[bucket];
    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);
            return data->value;
        }
        current = current->next;
    }
    char error_message[] = "Key not in table";
    std::__throw_runtime_error(error_message);
}

void HTable::remove(const std::string key) {
    u_long hash_val = hash(key.data(), key.size());
    long bucket = fmod(hash_val, size);

    Node *current = this->table[bucket];
    Node *prev = nullptr;

    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);

            if(prev){
                prev->next = current->next;
            }
            else{
                this->table[bucket] = current->next;
            }
            
            delete data;
            return;
        }
        prev = current;
        current = current->next;
    }
    char error_message[] = "Key not in table";
    std::__throw_runtime_error(error_message);
}

HTable::~HTable() {
    std::cout << "\nDestroyed\n";
}