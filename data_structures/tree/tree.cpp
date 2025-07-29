#include "tree.h"
#include <cstddef>

namespace data_structures{

    TreeNode::TreeNode(): left(nullptr), right(nullptr){};
    TreeNode::TreeNode(size_t frequency): frequency(frequency), left(nullptr), value(""), right(nullptr){};
    TreeNode::TreeNode(size_t frequency, std::string value): frequency(frequency), value(value), left(nullptr), right(nullptr){};

    TreeNode::~TreeNode(){
        delete this -> left;

        delete this -> right;

    }

    bool TreeNode::compare::operator()(const TreeNode * node_a, const TreeNode * node_b){
        return node_a -> frequency > node_b -> frequency;
    }
};
