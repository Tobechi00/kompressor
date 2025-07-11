#pragma once

#include "data_structures/tree/tree.h"
#include <queue>
#include <string>
#include <unordered_map>

class HuffmanCompression{

    std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> node_minheap;
    std::unordered_map<char, std::string> huffman_decode_map;
    std::string * file_content;

    public:
    HuffmanCompression(const std::string &n_file_content);

    std::unordered_map<char, std::string> &getHuffmanDecodeMap(){
        return this -> huffman_decode_map;
    }

    private:
    void populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);
    data_structures::TreeNode& constructHuffmanTree(std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);
    void populateDecodeMap(data_structures::TreeNode * huffman_tree, std::unordered_map<char, std::string> &huffman_map, std::string &huffman_code);
};
