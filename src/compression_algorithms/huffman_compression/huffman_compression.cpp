#include "huffman_compression.h"
#include "src/data_structures/tree/tree.h"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#define BYTE_SIZE 8

//rework encoding
HuffmanCompression::HuffmanCompression(const std::string &file_content){

    //construct huffman tree
    populateNodeMinHeap(file_content, this -> node_minheap);
    data_structures::TreeNode * huffman_tree = constructHuffmanTree(this -> node_minheap);

    //construct map
    size_t code_length = 0;
    std::string code_str("");
    populateBinaryMap(huffman_tree, this -> huffman_binary_map, code_str);//populate length

    delete huffman_tree;

    //convert and attach map as file header
    attachHeader(this -> binary_code, this -> huffman_binary_map);
    encodeContent(file_content, this -> binary_code, this -> huffman_binary_map);

    size_t old_size = file_content.length() * BYTE_SIZE;
    size_t new_size = this -> binary_code.length();

    std::cout << "old size: "<< old_size << "bits \n";
    std::cout << "new size: "<< new_size << "bits \n";

    if(new_size < old_size){
        size_t savings = old_size - new_size;
        std::cout << "total space saved: "<< savings << "bits \n";
    }else if(new_size == old_size){
        std::cout << "file saw no change with compression\n";
    }else{
        std::cout << "file saw a size increment with compression \n";
    }

}

/*
 * add values to min heap
 */
void HuffmanCompression::populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    std::unordered_map<std::string, size_t> frequency_map;

    std::string buffer;

    //sepearator for utf - 8 strings
    for(uint8_t c : file_content){
        std::bitset<BYTE_SIZE> bits(c);
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
void HuffmanCompression::populateBinaryMap(data_structures::TreeNode * huffman_tree, std::unordered_map<std::string, std::string> &huffman_map, std::string &code_str){
    if(huffman_tree -> left == nullptr && huffman_tree -> right == nullptr){
        huffman_map[huffman_tree -> value] =  code_str;
        return;
    }

    //generate length of each code for canonical huffman encoding
    code_str.push_back('1');
    populateBinaryMap(huffman_tree -> right, huffman_map, code_str);
    code_str.pop_back(); //backtrack

    code_str.push_back('0');
    populateBinaryMap(huffman_tree -> left, huffman_map, code_str);
    code_str.pop_back();
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
    std::string encoded_str;

    std::string buffer;

    for(const char c : content){
        buffer.push_back(c);
        auto map_iter = huffman_binary_map.find(buffer);

        if(map_iter != huffman_binary_map.end()){
            encoded_str.append(huffman_binary_map[buffer]);
            buffer.clear();
        }
    }

    binary_code.append(encoded_str);
}


std::string& HuffmanCompression::getBinaryCode(){
    return this -> binary_code;
}

void HuffmanCompression::attachHeader(std::string &binary_code, std::unordered_map<std::string, std::string> &huffman_binary_map){

    //flatten map (dictionary) to put in file
    // also attach 4byte length (32bit int)
    std::string flat_map;
    int bit_len = 32;//32 but not 100%, high diversity of utf-8 may break this specifies the length of our huffmancode

    for(const auto &pair : huffman_binary_map){

        for(uint8_t byte : pair.first){//convert char to byte rep
            std::bitset<BYTE_SIZE> bit_rep(byte);
            flat_map.append(bit_rep.to_string());
        }

        std::string code(pair.second);
        int code_len = code.length();

        // padding code incase to fit one byte;
        int remainder = bit_len - code_len;
        std::string padding_zeros(remainder, '0');
        code.append(padding_zeros);

        //converting actual code length to byte_rep
        uint8_t char_rep = static_cast<uint8_t>(code_len);
        std::bitset<BYTE_SIZE> len_bit_rep(code_len);


        //append code length
        flat_map.append(len_bit_rep.to_string());

        //append code itself
        flat_map.append(code);
    }

    int map_size = huffman_binary_map.size();//size of map not str

    //map size has a possibility to be very large but
    //thankfully nothing a good ol 32bit int can't hold

    uint32_t char_rep = static_cast<uint32_t>(map_size);
    std::bitset<32> maplen_bit_rep(char_rep);



    binary_code.append(maplen_bit_rep.to_string());
    binary_code.append(flat_map);
    //convert binary map to full binary
}
