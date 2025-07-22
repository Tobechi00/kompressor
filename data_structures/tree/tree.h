#pragma once

#include <cstddef>
namespace data_structures{

    struct TreeNode{// tree node def

        std::size_t frequency;
        char value;

        TreeNode * left;
        TreeNode * right;

        TreeNode();
        TreeNode(std::size_t n_frequency);
        TreeNode(std::size_t n_frequency, char n_value);

        ~TreeNode();

        public:
        struct compare{ // comparator for priority queue
            bool operator()(const TreeNode * node_a, const TreeNode * node_b);
        };
    };
}
