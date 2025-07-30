#include "table.hpp"
#include "tree.hpp"
class ZNode {
    private:
        TreeNode tree_node;
        Node table_node;
        double score;
        std::string name;
};

class ZSet {
    private:
        TreeNode *root;
        HMap map;    

    public:
        ZSet();
        bool zset_insert(std::string name, double score);
        ZNode *zset_lookup(std::string name);
        void zset_delete(std::string name);
        ~ZSet();
};
