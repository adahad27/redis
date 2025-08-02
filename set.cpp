#include "set.hpp"

ZSet::ZSet() {
    root = nullptr;
}

ZNode* alloc_znode(std::string name, int score) {
    u_long hash_val;
    std::string value = std::to_string(score);
    
    ZNode *node = new ZNode{
        name,
        value,
        EntryType::INT,
        {nullptr, nullptr, nullptr},
        {nullptr, hash_val}
    };
    return node;
}

ZNode* get_z_container(TreeNode *node) {
    return (ZNode*)((char*)node - offsetof(ZNode, tree_node));
}

ZNode* get_z_container(HNode *node) {
    return (ZNode*)((char*)node - offsetof(ZNode, table_node));
}



bool tree_less(TreeNode *node_1, TreeNode *node_2) {
    ZNode *znode_1 = get_z_container(node_1);
    ZNode *znode_2 = get_z_container(node_2);

    if(znode_1->value < znode_2->value) {
        return true;
    }
    else if(znode_1->value == znode_2->value) {
        return znode_1->key.compare(znode_2->key);
    }
    return false;

}

void update_tree(TreeNode **root, int score) {

}

bool ZSet::zset_insert(std::string name, int score) {
    
    if() {
        //Then special case for insertion
        update_tree(&root);
        return false;
    }
    ZNode *node = alloc_znode(name, score);
    map.insert_node(&node->table_node);
    search_and_insert(&root, &node->tree_node, tree_less);
    return true;
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
