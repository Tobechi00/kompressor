#pragma once

#include "data_structures/tree/tree.h"
#include <queue>
#include <string>
#include <unordered_map>

class HuffmanCompression{

    private:
    std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> node_minheap;
    std::unordered_map<std::string, std::string> huffman_binary_map;
    std::string * file_content;
    std::string binary_code;


    public:
    HuffmanCompression(const std::string &file_content);

    std::unordered_map<std::string, size_t> &getHuffmanLengthMap();
    std::string &getBinaryCode();


    private:
    void populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);

    data_structures::TreeNode * constructHuffmanTree(std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap);

    void populateBinaryMap(data_structures::TreeNode * huffman_tree, std::unordered_map<std::string, std::string> &huffman_map, std::string &code_str);

    void addOne(std::string &code);

    void encodeContent(const std::string &content, std::string &binary_code, std::unordered_map<std::string, std::string> &huffman_binary_map);

    void encodeContent(const std::string &content, std::string &binary_code);

    void attachHeader(std::string &binary_str, std::unordered_map<std::string, std::string> &huffman_binary_map);

    struct comparator {//priority queue comparator
        bool operator()(const std::pair<std::string,size_t> &left, const std::pair<std::string,size_t> &right);
    };

};
