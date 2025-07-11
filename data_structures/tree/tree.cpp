#include "tree.h"

namespace data_structures{

    TreeNode::TreeNode(): left(nullptr), right(nullptr){};
    TreeNode::TreeNode(int n_frequency): frequency(n_frequency), left(nullptr), right(nullptr){};
    TreeNode::TreeNode(int n_frequency, char n_value): frequency(n_frequency), value(n_value), left(nullptr), right(nullptr){};

    bool TreeNode::compare::operator()(const TreeNode * node_a, const TreeNode * node_b){
        return node_a -> frequency > node_b -> frequency;
    }
};
