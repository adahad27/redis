#include "table.hpp"
#include "tree.hpp"
struct ZNode {
    std::string key; 
    int value;   
    
    TreeNode tree_node;
    Node table_node;
};

class ZSet {
    private:
        TreeNode *root;
        HMap map;    

    public:
        ZSet();
        bool zset_insert(std::string name, int score);
        ZNode *zset_lookup(std::string name);
        void zset_delete(std::string name);
        ~ZSet();
};
