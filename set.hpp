#include "table.hpp"
#include "tree.hpp"
#include "objects.hpp"


class ZSet {
    private:
        TreeNode *root;
        HMap map;    

    public:
        ZSet();
        bool zset_insert(std::string key, int value);
        ZNode *zset_lookup(std::string key);
        void zset_delete(std::string key);
        ~ZSet();
};
