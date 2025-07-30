#include "table.hpp"
#include "tree.hpp"

class ZSet {
    private:
        TreeNode *root;
        HMap map;    

};

class ZNode {
    private:
        TreeNode tree_node;
        Node table_node;
        double score;
        uint32_t len;
        char name[MAX_MSG_LEN];
};