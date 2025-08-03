#ifndef OBJ
#define OBJ
#include <string>

enum EntryType {
    INIT = 0,
    STRING = 1,
    INT = 2
};

struct HNode {
    HNode *next;
    u_long hash = 0;
};

struct TreeNode {
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    uint32_t height;
};

struct Entry {
    std::string key;
    std::string value;
    EntryType type = EntryType::INIT;
    HNode table_node;
};




struct ZNode : Entry{
    TreeNode tree_node;
};



struct TreeDatum {
    u_long key;
    TreeNode node;
};

Entry* get_container(TreeNode *node_ptr);

Entry* get_container(HNode *node_ptr);

#endif