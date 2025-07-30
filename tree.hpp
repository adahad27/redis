#include <unistd.h>
#include <string>
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

void search_and_insert(TreeNode **root, TreeNode *new_node, bool (*less)(TreeNode *, TreeNode *));

TreeNode* search_and_delete(TreeNode **root, int32_t (*cmp)(TreeNode *, void *), void *key);