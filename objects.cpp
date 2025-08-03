#include "objects.hpp"
#include <cstddef>

Entry* get_container(HNode *node_ptr) {
    if(node_ptr) {
        return (Entry*)((char*)node_ptr - offsetof(Entry, table_node));
    }
    return nullptr;
}

Entry* get_container(TreeNode *node_ptr) {
    if(node_ptr) {
        return (ZNode*)((char*)node_ptr - offsetof(ZNode, tree_node));
    }
    return nullptr;
}