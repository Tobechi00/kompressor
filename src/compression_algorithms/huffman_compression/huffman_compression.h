#ifndef _HUFFMAN_COMPRESSION_H
#define _HUFFMAN_COMPRESSION_H

#include "src/data_structures/tree/tree.h"
#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>

class HuffmanCompression{

    private:
    std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> node_minheap;
    std::unordered_map<std::string, std::string> huffman_binary_map;
    std::string binary_code;


    public:
    HuffmanCompression(std::string &file_path);

    private:
    //compression structure creators
    data_structures::TreeNode * constructHuffmanTree(
        std::priority_queue<data_structures::TreeNode*,
        std::vector<data_structures::TreeNode*>,
        data_structures::TreeNode::compare> &node_minheap
    );

    void populateBinaryMap(
        data_structures::TreeNode * huffman_tree,
        std::unordered_map<std::string,
        std::string> &huffman_map, std::string &code_str
    );

    void buffPopulateNodeMinHeap(
        std::ifstream &file, std::priority_queue<data_structures::TreeNode*,
        std::vector<data_structures::TreeNode*>,
        data_structures::TreeNode::compare> &node_minheap);

    void populateNodeMinHeap(
        const std::string &file_content,
        std::unordered_map<std::string, size_t> &frequency_map,
        std::priority_queue<data_structures::TreeNode*,
        std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap
    );

    //encoders
    void buffEncodeContent(
        std::ifstream &file,
        std::ofstream &out_file,
        std::unordered_map<std::string,
        std::string> &huffman_binary_map
    );

    std::string encodeContent(
        std::string &content,
        std::unordered_map<std::string, std::string> &huffman_binary_map
    );

    void attachHeader(
        std::ofstream &out_file,
        std::unordered_map<std::string,
        std::string> &huffman_binary_map
    );
    void writeChar(std::string &bits, std::ofstream &outfile);

    //priority queue comparator
    struct comparator {
        bool operator()(const std::pair<std::string,size_t> &left, const std::pair<std::string,size_t> &right);
    };

};

#endif
