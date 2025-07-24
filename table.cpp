#include "table.hpp"
#include <iostream>
#define DEBUG(x) std::cout<<"Reached line " << x << std::endl;
#define FNV_BASIS 14695981039346656037u
#define FNV_OFFSET 1099511628211


Datum* get_container(Node *node_ptr) {
    return (Datum*)((char*)node_ptr - offsetof(Datum, node));
}


u_long HTable::hash(const char *key, uint32_t size) {
    uint64_t current_hash = FNV_BASIS;

    for(uint32_t i = 0; i < size; ++i) {
        current_hash = current_hash ^ key[i];
        current_hash *= FNV_OFFSET;
    }
    return current_hash;

}

Datum* HTable::return_datum(const std::string key) {
    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val % size;

    Node *current = this->table[bucket];
    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);
            return data;
        }
        current = current->next;
    }
    return nullptr;
}

HTable::HTable(uint32_t size) {
    this->table = new Node*[size];
    for(uint32_t i = 0; i < size; ++i) {
        table[i] = nullptr;
    }
    this->size = size;
}


void HTable::insert(const std::string key, const std::string value) {
    //TODO: Insert check for growing logic here


    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val % size;

    /* TODO: Currently hash for this key is calculated twice, once in insert and
    once in return_datum(). Only needs to be called once. */
    Datum *data = return_datum(key);
    if(data == nullptr) {
        data = new Datum{value, {nullptr, hash_val}};
        data->node.next = this->table[bucket];
        this->table[bucket] = &data->node;
    }
    else {
        data->value = value;
    }

    
    

}


std::string HTable::get(const std::string key) {
    Datum *data = return_datum(key);
    if(data != nullptr) {
        return data->value;
    }
    char error_message[] = "Key not in table";
    std::__throw_runtime_error(error_message);
}


bool HTable::contains(const std::string key) {
    return return_datum(key) != nullptr;
}


void HTable::remove(const std::string key) {
    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val % size;

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
    //Do we need this behavior if the key doesn't exist? I don't think so
    // char error_message[] = "Key not in table";
    // std::__throw_runtime_error(error_message);
}

void destroy_bucket(Node *node) {
    if(node != nullptr) {
        destroy_bucket(node->next);
        node->next = nullptr;
        Datum *data = get_container(node);
        delete data;
    }
}


HTable::~HTable() {
    if(table != nullptr) {
        for(uint32_t i = 0; i < size; ++i) {
            destroy_bucket(table[i]);
            table[i] = nullptr;
        }
    }
    delete[] table;
    table = nullptr;
}

HTable& HTable::operator=(HTable &rhs) {
    this->~HTable();
    this->table = rhs.table;
    this->size = rhs.size;
    
    rhs.size = 0;
    rhs.table = nullptr;
    return *this;
}

std::string HMap::get(const std::string key) {
    Datum *data;
    data = old_table.return_datum(key);
    if(data != nullptr) {
        return data->value;
    }
    data = new_table.return_datum(key);
    if(data != nullptr) {
        return data->value;
    }
    char error_message[] = "Key not in table";
    std::__throw_runtime_error(error_message);
}


bool HMap::contains(const std::string key) {
    if(old_table.return_datum(key) != nullptr) {
        return true;
    }
    if(new_table.return_datum(key) != nullptr) {
        return true;
    }
    return false;
}


void HMap::remove(const std::string key) {
    old_table.remove(key);
    new_table.remove(key);
}