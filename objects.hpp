#include <string>

enum EntryType {
    INIT = 0,
    STRING = 1,
    INT = 2
};

struct Entry {
    std::string key;
    std::string value;
    EntryType type = EntryType::INIT;
};

struct HNode {
    HNode *next;
    u_long hash = 0;
};

struct Datum : Entry{
    HNode node;
};

struct ZNode : Entry{
    
    TreeNode tree_node;
    HNode table_node;
};

struct TreeNode {
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    uint32_t height;
};

struct TreeDatum {
    u_long key;
    TreeNode node;
};