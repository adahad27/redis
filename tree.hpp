#include <unistd.h>
#include <string>
#include "objects.hpp"

void search_and_insert(TreeNode **root, TreeNode *new_node, bool (*less)(TreeNode *, TreeNode *));

TreeNode* search_and_delete(TreeNode **root, int32_t (*cmp)(TreeNode *, void *), void *key);