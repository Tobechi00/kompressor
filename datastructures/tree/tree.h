#pragma once

namespace datastructures{

    struct TreeNode{// tree node def

        int frequency;
        char value;

        TreeNode * left;
        TreeNode * right;

        TreeNode();
        TreeNode(int n_frequency);
        TreeNode(int n_frequency, char n_value);

        struct compare{ // comparator for priority queue
            bool operator()(const TreeNode * node_a, const TreeNode * node_b);
        };
    };
}
