#pragma once

#include <cstddef>
#include <string>

namespace data_structures{

    struct TreeNode{

        std::size_t frequency;
        std::string value;

        TreeNode * left;
        TreeNode * right;

        TreeNode();
        TreeNode(std::size_t frequency);
        TreeNode(std::size_t frequency, std::string value);

        ~TreeNode();

        public:
        struct compare{ // comparator for priority queue
            bool operator()(const TreeNode * node_a, const TreeNode * node_b);
        };
    };
}
