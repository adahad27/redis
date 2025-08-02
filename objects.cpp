#include "objects.hpp"
#include <cstddef>

inline Entry* get_container(HNode *node_ptr) {
    return (Entry*)((char*)node_ptr - offsetof(Entry, table_node));
}

inline Entry* get_container(TreeNode *node_ptr) {
    return (ZNode*)((char*)node_ptr - offsetof(ZNode, tree_node));
}