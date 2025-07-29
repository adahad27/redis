#include "tree.hpp"

void init_node(TreeNode *node) {
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 0;
}

uint32_t get_height(TreeNode *node) {
    return node? node->height : 0;
}

uint32_t get_height_diff(TreeNode *node) {
    return node? get_height(node->left) - get_height(node->right) : 0;
}

void update_height(TreeNode *node) {
    node->height = 1 + std::max(node->left->height, node->right->height);
}


