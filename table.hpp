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
        uint32_t mask;
        uint32_t size;

        /* hash() returns a hash based on the FNV-1a hashing algorithm for a 
        sequence of bytes. */
        u_long hash(const char *key, uint32_t size);

        /* return_datum() will return a pointer to the Datum object
        that corresponds to key. If the key is not in the table, returns nullptr
        instead. */
        Datum* return_datum(const std::string key);

        void insert_node(Node *node);

        Node *pop_node(const std::string key);
    
    public:

        HTable();

        HTable(uint32_t capacity);

        void init_table(uint32_t capacity);

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


        HTable& operator=(HTable &rhs);

        friend class HMap;

};


class HMap {
    private:
        HTable old_table;
        HTable new_table;
        uint32_t current_bucket;
        uint32_t load_factor_limit;
        void shift_items(uint32_t num_items);
        void shift_item();

    public:
        HMap();
        HMap(uint32_t size);
        void insert(const std::string key, const std::string value);
        std::string get(const std::string key);
        bool contains(const std::string key);
        void remove(const std::string key);

        void insert_node(Node *node);
        Node* pop_node(const std::string key);

        ~HMap();
};





#endif