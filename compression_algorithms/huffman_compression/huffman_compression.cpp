#include "huffman_compression.h"
#include "data_structures/tree/tree.h"
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

//todo: change mapping to string to code to account for utf - 8 encoding, accumilator code at the bottom;


HuffmanCompression::HuffmanCompression(const std::string &file_content){

    populateNodeMinHeap(file_content, this -> node_minheap);


    if(this -> node_minheap.size() == 1){//edge case where we only have one element in our heap //(i.e only one unique element)

        data_structures::TreeNode * only_node = this -> node_minheap.top();

        std::string only_char = only_node-> value;

        this -> huffman_length_map[only_char] = 1; //assign minimal length

        this -> node_minheap.pop();

        delete only_node;
    }else{
        data_structures::TreeNode * huffman_tree = constructHuffmanTree(this -> node_minheap);
        size_t code_length = 0;

        populateLengthMap(huffman_tree, this -> huffman_length_map, code_length);//populate length

        convertLengthMapToBinary(this -> huffman_length_map, this -> huffman_binary_map);
        delete huffman_tree;
    }

    this -> binary_code = new std::string();

    encodeContent(file_content, *this -> binary_code, this -> huffman_binary_map);

    for(auto &pair : this -> huffman_binary_map){
        std::cout << pair.first << " -- " << pair.second << "\n";
    }


    std::cout << binary_code << '\n';
    std::cout << "old size: "<< (file_content.length() * 8) << "bits \n";
    std::cout << "new size: "<< ( (*this -> binary_code).length()) << "bits \n";

}

/*
 * add values to min heap
 */
void HuffmanCompression::populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    std::unordered_map<std::string, size_t> frequency_map;

    std::string buffer;

    //sepearator for utf - 8 strings
    for(uint8_t c : file_content){
        std::bitset<8> bits(c);
        std::string bitval = bits.to_string();

        if(bitval.at(0) == '1' && bitval.at(1) == '0'){
            buffer.push_back(c);
        }else{
            if(buffer.empty()){
                buffer.push_back(c);
            }else{
                frequency_map[buffer]++;
                buffer.clear();
                buffer.push_back(c);
            }
        }
    }


    if(!buffer.empty()){
        frequency_map[buffer]++;
         //buffer.clear;
    }

    for(const auto &map_pair : frequency_map){//loop through address
        data_structures::TreeNode * node = new data_structures::TreeNode(map_pair.second, map_pair.first);
        node_minheap.push(node);
    }
}

/*
 * construct huffman tree using node min_heap;
 */

data_structures::TreeNode * HuffmanCompression::constructHuffmanTree(std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    while(node_minheap.size() > 1){
        data_structures::TreeNode * combination_node;

        data_structures::TreeNode * first_node = node_minheap.top();
        node_minheap.pop();//remove ptr from queue

        data_structures::TreeNode * second_node = node_minheap.top();
        node_minheap.pop();//remove ptr from queue

        size_t sum_frequency = first_node -> frequency + second_node -> frequency;
        combination_node = new data_structures::TreeNode(sum_frequency);//combine node frequency and create new nodesecon

        combination_node -> left = first_node;
        combination_node -> right = second_node; //assign left and right nodes to new tree;

        node_minheap.push(combination_node);
    }

    data_structures::TreeNode * head_node = node_minheap.top();
    node_minheap.pop();

    return head_node;
}

/*
 * create decode map which allows for decompression
 */
void HuffmanCompression::populateLengthMap(data_structures::TreeNode * huffman_tree, std::unordered_map<std::string, size_t> &huffman_map, size_t code_length){
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
void HuffmanCompression::convertLengthMapToBinary(std::unordered_map<std::string, size_t> &str_to_length_map, std::unordered_map<std::string, std::string> &huffman_binary_map){
    std::vector<std::pair<std::string, size_t>> token_frequency_list;

    for(const auto &pair : str_to_length_map){
        token_frequency_list.push_back(pair);
    }

    //use vector to sort list based on code length
    std::sort(token_frequency_list.begin(), token_frequency_list.end(), HuffmanCompression::comparator());

    const std::pair<std::string, size_t> * previous = nullptr; //posize_ter to previous element

    for(const auto &current : token_frequency_list){

        if(previous == nullptr){
            huffman_binary_map[current.first] = std::string(current.second, '0');

            previous = &current;
        }else{


            std::string code = huffman_binary_map[previous -> first]; //get previous code

            addOne(code);

            if(current.second > previous -> second){
                //add zero to string equivalent to the difference of the previous and current code length
                size_t zeros = current.second - previous -> second;

                for(size_t i = 0; i < zeros; i++){
                    code.push_back('0');
                }
            }

            huffman_binary_map[current.first] = code;
            previous = &current;
        }
    }
}

//comparator sorts by code length then by character if code length is the same
bool HuffmanCompression::comparator::operator()(const std::pair<std::string, size_t> &left, const std::pair<std::string, size_t> &right) {

    if(left.second == right.second){
        return left.first.length() < right.first.length();
    }

    return left.second < right.second;
}

//used to increment binary representation
void HuffmanCompression::addOne(std::string &code){

    bool has_remainder = false;

    for(size_t i = code.length() - 1; i >= 0; i--){

        if(!has_remainder){
            if(code[i] == '0'){
                code[i] = '1';
                break;
            }else{
                code[i] = '0';
                has_remainder = true;
            }
        }else{
            if(code[i] == '0'){
                code[i] = '1';
                has_remainder = false;
                break;
            }else{
                code[i] = '0';
            }
        }
    }

    if(has_remainder){
        code = '1'+code;
    }
}


//used to encode plain text to huffman coded binary
void HuffmanCompression::encodeContent(const std::string &content, std::string &binary_code, std::unordered_map<std::string, std::string> &huffman_binary_map){

    std::string buffer;

    for(uint8_t c : content){
        std::bitset<8> bits(c);
        std::string bitval = bits.to_string();

        if(bitval.at(0) == '1' && bitval.at(1) == '0'){
            buffer.push_back(c);
        }else{
            if(buffer.empty()){
                buffer.push_back(c);
            }else{
                binary_code.append(buffer);
                buffer.clear();
                buffer.push_back(c);
            }
        }
    }


    if(!buffer.empty()){
        binary_code.append(buffer);
    }
}

std::unordered_map<std::string, size_t>& HuffmanCompression::getHuffmanLengthMap(){
    return this -> huffman_length_map;
}


std::string& HuffmanCompression::getBinaryCode(){
    return (*this -> binary_code);
}
