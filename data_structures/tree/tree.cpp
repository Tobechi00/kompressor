#include "tree.h"
#include <cstddef>

namespace data_structures{

    TreeNode::TreeNode(): left(nullptr), right(nullptr){};
    TreeNode::TreeNode(size_t n_frequency): frequency(n_frequency), left(nullptr), value('\0'), right(nullptr){};
    TreeNode::TreeNode(size_t n_frequency, char n_value): frequency(n_frequency), value(n_value), left(nullptr), right(nullptr){};

    TreeNode::~TreeNode(){
        delete this -> left;

        delete this -> right;

    }

    bool TreeNode::compare::operator()(const TreeNode * node_a, const TreeNode * node_b){
        return node_a -> frequency > node_b -> frequency;
    }
};
