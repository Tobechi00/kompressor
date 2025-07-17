#include "huffman_compression.h"
#include "data_structures/tree/tree.h"
#include <algorithm>
#include <bitset>
#include <queue>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

//todo: rework add one function


HuffmanCompression::HuffmanCompression(const std::string &n_file_content){

    std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap = HuffmanCompression::node_minheap;
    std::unordered_map<char, int> &huffman_length_map = HuffmanCompression::huffman_length_map;
    std::unordered_map<char, std::string> &huffman_binary_map = HuffmanCompression::huffman_binary_map;


    populateNodeMinHeap(n_file_content, node_minheap);

    if(node_minheap.size() == 1){//edge case where we only have one element in our heap //(i.e only one unique element)

        data_structures::TreeNode * only_node = node_minheap.top();

        char only_char = only_node-> value;

        huffman_length_map[only_char] = 1; //assign minimal length

        node_minheap.pop();

        delete only_node;
        only_node = nullptr;
    }else{
        data_structures::TreeNode * huffman_tree = &constructHuffmanTree(node_minheap);
        int code_length = 0;

        populateLengthMap(huffman_tree, huffman_length_map, code_length);//populate length

        convertLengthMapToBinary(huffman_length_map, huffman_binary_map);
    }

    for(const auto &pair : HuffmanCompression::huffman_binary_map){
        std::cout << pair.first <<" -- "<< pair.second << '\n';
    }

}


/*
 * add values to min heap
 */
void HuffmanCompression::populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    std::unordered_map<char, int> frequency_map;

    for(const char c : file_content){//generate frequency map
        frequency_map[c]++;
    }

    for(const auto &map_pair : frequency_map){//loop through address
        data_structures::TreeNode * node = new data_structures::TreeNode(map_pair.second, map_pair.first);
        node_minheap.push(node);
    }
}

/*
 * construct huffman tree using node min_heap;
 */

data_structures::TreeNode& HuffmanCompression::constructHuffmanTree(std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    while(node_minheap.size() > 1){
        data_structures::TreeNode * combination_node;

        data_structures::TreeNode * first_node = node_minheap.top();
        node_minheap.pop();//remove ptr from queue

        data_structures::TreeNode * second_node = node_minheap.top();
        node_minheap.pop();//remove ptr from queue

        int sum_frequency = first_node -> frequency + second_node -> frequency;
        combination_node = new data_structures::TreeNode(sum_frequency);//combine node frequency and create new nodesecon

        combination_node -> left = first_node;
        combination_node -> right = second_node; //assign left and right nodes to new tree;

        node_minheap.push(combination_node);
    }

    return *node_minheap.top();
}

/*
 * create decode map which allows for decompression
 */
void HuffmanCompression::populateLengthMap(data_structures::TreeNode * huffman_tree, std::unordered_map<char, int> &huffman_map, int code_length){
    if(huffman_tree -> left == nullptr && huffman_tree -> right == nullptr){
        huffman_map[huffman_tree -> value] =  code_length;
        return;
    }

    //generate length of each code for canonical huffman encoding
    code_length++;
    populateLengthMap(huffman_tree -> right, huffman_map, code_length);
    code_length--; //backtrack

    code_length++;
    populateLengthMap(huffman_tree -> left, huffman_map, code_length);
    code_length++;
}


//generates length to huffman code
//
void HuffmanCompression::convertLengthMapToBinary(std::unordered_map<char, int> &char_to_length_map, std::unordered_map<char, std::string> &huffman_binary_map){
    std::vector<std::pair<char, int>> token_frequency_list;

    for(const auto &pair : char_to_length_map){
        token_frequency_list.push_back(pair);
    }

    //use vector to sort list based on
    std::sort(token_frequency_list.begin(), token_frequency_list.end(), HuffmanCompression::comparator());

    const std::pair<char, int> * previous = nullptr; //pointer to previous element

    for(const auto &current : token_frequency_list){

        if(previous == nullptr){
            huffman_binary_map[current.first] = std::string(current.second, '0');

            previous = &current;
        }else{


            std::string code = huffman_binary_map[previous -> first]; //get previous code

            addOne(code);

            if(current.second > previous -> second){
                //add zero to string equivalent to the difference of the previous and current code length
                int zeros = current.second - previous -> second;

                for(int i = 0; i < zeros; i++){
                    code.push_back('0');
                }
            }

            huffman_binary_map[current.first] = code;
            previous = &current;
        }
    }
}

//comparator sorts by code length then by character if code length is the same
bool HuffmanCompression::comparator::operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {

    if(left.second == right.second){
        return left.first < left.second;
    }

    return left.second < right.second;
}

//used to increment binary representations
void HuffmanCompression::addOne(std::string &code){

    int len = code.length();
    int num_code = std::stoi(code, nullptr, 2);
    num_code++;
    std::bitset<32> binary_rep(num_code);

    code = binary_rep.to_string().substr(32 - len);
}
