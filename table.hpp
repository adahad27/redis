#ifndef TBL
#define TBL
#include "msg_lib.hpp"
#include <string>
#include <memory>
#include <cmath>

struct Node {
    Node *next;
    u_long hash = 0;
};

struct Datum {
    std::string value;
    Node node;
};

/* get_container() returns the pointer to the start of the intrusive data
structure that holds node_ptr. */
Datum* get_container(Node *node);

class HTable {
    private:
        Node **table;
        uint32_t size;

        /* hash() returns a hash based on the FNV-1a hashing algorithm for a 
        sequence of bytes. */
        u_long hash(const char *key, uint32_t size);
    
    public:
        HTable(uint32_t size);

        /* insert() inserts the key, value pair into the hashtable. If the value
        already exists, then the value is replaced */
        void insert(const std::string key, const std::string value);

        /* get() returns the corresponding value associated with the given key. 
        If the key does not exist in the table, a runtime error is thrown. */
        std::string get(const std::string key);

        /* contains() returns the truth value of the given key being contained 
        within the table. */
        bool contains(const std::string key);

        /* remove() deletes the key-value object from the table. */
        void remove(const std::string key);

        /* Destructor is responsible for manually cleaning up all memory that is
         present within the table. */
        ~HTable();


};





#endif