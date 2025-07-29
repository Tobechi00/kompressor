#pragma once

#include "data_structures/tree/tree.h"
#include <queue>
#include <string>
#include <unordered_map>

class HuffmanCompression{

    std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> node_minheap;
    std::unordered_map<std::string, size_t> huffman_length_map;
    std::unordered_map<std::string, std::string> huffman_binary_map;
    std::string * file_content;
    std::string * binary_code;

    public:
    HuffmanCompression(const std::string &n_file_content);

    std::unordered_map<std::string, size_t> &getHuffmanLengthMap();
    std::string &getBinaryCode();

    private:
    void populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);

    data_structures::TreeNode * constructHuffmanTree(std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);

    void populateLengthMap(data_structures::TreeNode * huffman_tree, std::unordered_map<std::string, size_t> &huffman_map, size_t code_length);

    void convertLengthMapToBinary(std::unordered_map<std::string, size_t> &length_map, std::unordered_map<std::string, std::string> &huffman_binary_map);

    void addOne(std::string &code);

    void encodeContent(const std::string &content, std::string &binary_code, std::unordered_map<std::string, std::string> &huffman_binary_map);

    struct comparator {
        bool operator()(const std::pair<std::string,size_t> &left, const std::pair<std::string,size_t> &right);
    };

};
