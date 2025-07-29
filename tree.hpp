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

