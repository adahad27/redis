#include "set.hpp"

ZSet::ZSet() {
    root = nullptr;
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
