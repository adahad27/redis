#include "table.hpp"
#include <iostream>
#include <cassert>
#define DEBUG(x) std::cout<<"Reached line " << x << std::endl;
#define FNV_BASIS 14695981039346656037u
#define FNV_OFFSET 1099511628211


Datum* get_container(HNode *node_ptr) {
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
    if(table == nullptr){
        return nullptr;
    }
    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val & mask;

    HNode *current = table[bucket];
    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);
            return data;
        }
        current = current->next;
    }
    return nullptr;
}

HTable::HTable() {
    table = nullptr;
    size = 0;
    mask = 0;
}

HTable::HTable(uint32_t capacity) {
    assert(capacity > 0 && ((capacity - 1) & capacity) == 0);
    table = new HNode*[capacity];
    for(uint32_t i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
    mask = capacity - 1;
    size = 0;
}

void HTable::init_table(uint32_t capacity) {
    assert(capacity > 0 && ((capacity - 1) & capacity) == 0);
    this->~HTable();
    table = new HNode*[capacity];
    for(uint32_t i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
    mask = capacity - 1;
    size = 0;
}

void HTable::insert(const std::string key, const std::string value) {
    if(table == nullptr){
        //TODO: Think more about if this is the best behavior.
        char error_message[] = "Cannot insert into uninitialized table";
        std::__throw_runtime_error(error_message);
    }


    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val & mask;

    /* TODO: Currently hash for this key is calculated twice, once in insert and
    once in return_datum(). Only needs to be called once. */
    Datum *data = return_datum(key);
    if(data == nullptr) {
        data = new Datum{value, {nullptr, hash_val}};
        data->node.next = table[bucket];
        table[bucket] = &data->node;
        size += 1;
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
    DEBUG("105");
    char error_message[] = "Key not in table";
    std::__throw_runtime_error(error_message);
}


bool HTable::contains(const std::string key) {
    return return_datum(key) != nullptr;
}


void HTable::remove(const std::string key) {
    if(table == nullptr) {
        return;
    }
    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val & mask;

    HNode *current = table[bucket];
    HNode *prev = nullptr;

    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);

            if(prev){
                prev->next = current->next;
            }
            else{
                table[bucket] = current->next;
            }
            
            delete data;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void destroy_bucket(HNode *node) {
    if(node != nullptr) {
        destroy_bucket(node->next);
        node->next = nullptr;
        Datum *data = get_container(node);
        delete data;
    }
}


HTable::~HTable() {
    if(table != nullptr) {
        for(uint32_t i = 0; i <= mask; ++i) {
            destroy_bucket(table[i]);
            table[i] = nullptr;
        }
    }
    delete[] table;
    table = nullptr;
}

HTable& HTable::operator=(HTable &rhs) {
    this->~HTable();
    table = rhs.table;
    mask = rhs.mask;
    size = rhs.size;
    
    rhs.size = 0;
    rhs.table = nullptr;
    return *this;
}

void HTable::insert_node(HNode *node) {
    //Assume the hash value is already set.
    ulong hash_val = node->hash;
    uint32_t bucket = hash_val & mask;
    HNode *current = table[bucket];

    while(current != nullptr) {
        if(current->next && current->next->hash == hash_val) {
            HNode *target = current->next;
            node->next = target->next;
            current->next = node;
            delete get_container(target);
            return;
        }

        current = current->next;
    }

    node->next = current;
    table[bucket] = node;

}

HNode* HTable::pop_node(const std::string key) {
    if(table == nullptr) {
        return nullptr;
    }
    u_long hash_val = hash(key.data(), key.size());
    uint32_t bucket = hash_val & mask;

    HNode *current = table[bucket];
    HNode *prev = nullptr;

    while(current != nullptr) {
        if(current->hash == hash_val){
            Datum *data = get_container(current);

            if(prev){
                prev->next = current->next;
            }
            else{
                table[bucket] = current->next;
            }
            
            return current;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

HMap::HMap() {
    new_table.init_table(1);
    current_bucket = 0;
    load_factor_limit = 3;
}

HMap::HMap(uint32_t size) {
    new_table.init_table(size);
    current_bucket = 0;
    load_factor_limit = 3; //For maximum allowable load factor
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
    char error_message[] = "Key not in Map";
    DEBUG("195");
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

void HMap::insert(const std::string key, const std::string value) {
    new_table.insert(key, value);
    if(new_table.size > (new_table.mask + 1) * load_factor_limit) {
        //start rehashing
        old_table = new_table;
        uint32_t mask = old_table.mask;
        mask = (mask + 1) << 1;
        new_table.init_table(mask); //Double new table's size

    }
    shift_items(128);
}


void HMap::shift_item() {
    Datum *data = get_container(old_table.table[current_bucket]);
    u_long hash_value = data->node.hash;
    old_table.table[current_bucket] = data->node.next;

    
    uint32_t bucket = hash_value & new_table.mask;

    // If the hashed value exists in new_table, return without doing anything
    HNode *current = new_table.table[bucket];
    while(current != nullptr) {
        if(current->hash == hash_value) {
            return;
        }
        current = current->next;
    }
    data->node.next = new_table.table[bucket];
    new_table.table[bucket] = &data->node;

}


void HMap::shift_items(uint32_t num_items) {
    HNode **table = old_table.table;
    if(table == nullptr) {
        return;
    }
    uint32_t capacity = old_table.mask;
    for(uint32_t i = 0; i < num_items; ++i) {
        if(table[current_bucket] == nullptr && current_bucket == capacity) {
            current_bucket = 0;
            old_table.~HTable();
            return;
        }
        if(table[current_bucket] == nullptr) {
            current_bucket += 1;
            continue;
        }
        shift_item();
    }
}

HMap::~HMap() {
    old_table.~HTable();
    new_table.~HTable();
}

void HMap::insert_node(HNode *node) {
    new_table.insert_node(node);
}

HNode* HMap::pop_node(const std::string key) {
    HNode *node = old_table.pop_node(key);
    if(node) {
        return node;
    }
    else {
        return new_table.pop_node(key);
    }
}