#include "set.hpp"

ZSet::ZSet() {
    root = nullptr;
}

ZNode* alloc_znode(std::string name, int score) {
    u_long hash_val;
    ZNode *node = new ZNode{
        name,
        score,
        {nullptr, nullptr, nullptr},
        {nullptr, hash_val}
    };
    return node;
}

bool ZSet::zset_insert(std::string name, int score) {
    if(map.contains(name)) {
        //Then special case for insertion

        return false;
    }
    ZNode *node = alloc_znode(name, score);
    map.insert_node(&node->table_node);
    search_and_insert();
}

ZNode* get_tree_container(TreeNode *node) {

}

void delete_tree(TreeNode *node) {
    if(node) {
        delete_tree(node->left);
        delete_tree(node->right);
        node->parent = nullptr;
        ZNode *data = get_tree_container(node);
        delete data;
    }
}

ZSet::~ZSet() {
    delete_tree(root);
}
