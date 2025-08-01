#include "table.hpp"
#include "tree.hpp"
#include "objects.hpp"


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
