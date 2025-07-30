#include "tree.hpp"
#include <cassert>

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

TreeNode* rot_left(TreeNode *node) {
    TreeNode *parent = node->parent;
    TreeNode *new_node = node->right;
    TreeNode *inner = new_node->left;
    node->right = inner;
    if (inner) {
        inner->parent = node;
    }
    new_node->parent = parent;
    new_node->left = node;
    node->parent = new_node;
    update_height(node);
    update_height(new_node);
    return new_node;
}

TreeNode* rot_right(TreeNode *node) {
    TreeNode *parent = node->parent;
    TreeNode *new_node = node->left;
    TreeNode *inner = new_node->right;
    node->left = inner;
    if (inner) {
        inner->parent = node;
    }
    new_node->parent = parent;
    new_node->right = node;
    node->parent = new_node;
    update_height(node);
    update_height(new_node);
    return new_node;
}


TreeNode *fix_left(TreeNode *node) {
    if (get_height(node->left->left) < get_height(node->left->right)) {
        node->left = rot_left(node->left);
    }
    return rot_right(node);
}

TreeNode *fix_right(TreeNode *node) {
    if (get_height(node->right->left) > get_height(node->right->right)) {
        node->right = rot_right(node->right);
    }
    return rot_left(node);
}

TreeNode *fix(TreeNode *node) {
    while (true) {
        TreeNode **from = &node;
        TreeNode *parent = node->parent;
        if (parent) {
            from = parent->left == node ? &parent->left : &parent->right;
        }
        update_height(node);
        uint32_t l = get_height(node->left);
        uint32_t r = get_height(node->right);
        if (l == r + 2) {
            *from = fix_left(node);
        } else if (l + 2 == r) {
            *from = fix_right(node);
        }
        if (!parent) {
            return *from;
        }
        node = parent;
    }
}


static TreeNode *del_empty(TreeNode *node) {
    assert(!node->left || !node->right);
    TreeNode *child = node->left ? node->left : node->right;
    TreeNode *parent = node->parent;
    if (child) {
        child->parent = parent;
    }
    if (!parent) {
        return child;
    }
    TreeNode **from = parent->left == node ? &parent->left : &parent->right;
    *from = child;
    return fix(parent);
}


TreeNode *del(TreeNode *node) {
    if (!node->left || !node->right) {
        return del_empty(node);
    }
    TreeNode *victim = node->right;
    while (victim->left) {
        victim = victim->left;
    }
    TreeNode *root = del_empty(victim);
    *victim = *node;
    if (victim->left) {
        victim->left->parent = victim;
    }
    if (victim->right) {
        victim->right->parent = victim;
    }
    TreeNode **from = &root;
    TreeNode *parent = node->parent;
    if (parent) {
        from = parent->left == node ? &parent->left : &parent->right;
    }
    *from = victim;
    return root;
}

void search_and_insert(TreeNode **root, TreeNode *new_node, bool (*less)(TreeNode *, TreeNode *))
{
    TreeNode *parent = nullptr;
    TreeNode **from = root;
    for (TreeNode *node = *root; node; ) {
        from = less(new_node, node) ? &node->left : &node->right;
        parent = node;
        node = *from;
    }
    *from = new_node;
    new_node->parent = parent;
    *root = fix(new_node);
}

TreeNode* search_and_delete(TreeNode **root, int32_t (*cmp)(TreeNode *, void *), void *key)
{
    for (TreeNode *node = *root; node; ) {
        int32_t r = cmp(node, key);
        if (r < 0) {
            node = node->right;
        } else if (r > 0) {
            node = node->left;
        } else {
            *root = del(node);
            return node;
        }
    }
    return nullptr;
}