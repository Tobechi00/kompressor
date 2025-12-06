#include "huffman_compression.h"
#include "src/data_structures/tree/tree.h"
#include "src/util/util.h"
#include "src/util/util.h"
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

HuffmanCompression::HuffmanCompression(std::string &file_path){

    //open read file
    std::ifstream file(file_path);

    if(!file.is_open()){
        std::cerr << "an error occurred while opening file" << '\n';
        return;
    }

    //construct huffman tree
    buffPopulateNodeMinHeap(file, this -> node_minheap);
    data_structures::TreeNode * huffman_tree = constructHuffmanTree(this -> node_minheap);


    //construct map
    std::string code_str("");
    populateBinaryMap(huffman_tree, this -> huffman_binary_map, code_str);//populate length

    delete huffman_tree;

    //open out_file
    std::ofstream out_file(
        util::generateCompressionOut(file_path),
        std::ios::binary | std::ios::trunc
    );

    if(!out_file.is_open()){
        std::cerr << "error opening output file";
        return;
    }

    //convert and attach map as file header
    attachHeader(out_file, this -> huffman_binary_map);

    //reset file pointer
    file.clear();
    file.seekg(0, file.beg);

    //rework buffer endcoding and writing to disk dont hold code in memory
    buffEncodeContent(file, out_file, this -> binary_code, this -> huffman_binary_map);

    file.clear();

    file.seekg(0, std::ifstream::end);
    out_file.seekp(0, std::ofstream::end);

    std::cout << this -> huffman_binary_map.size() << "\n";

    size_t old_size = file.tellg();
    // change
    size_t new_size = out_file.tellp();

    std::cout << "old size: "<< old_size << "bytes \n";
    std::cout << "new size: "<< new_size << "bytes \n";

    if(new_size < old_size){
        size_t savings = old_size - new_size;
        std::cout << "total space saved: "<< savings << "bytes \n";
    }else if(new_size == old_size){
        std::cout << "file saw no change with compression\n";
    }else{
        std::cout << "file saw a size increment with compression \n";
    }

    file.close();
    out_file.close();
}


void HuffmanCompression::populateNodeMinHeap(
    const std::string &file_content,
    std::unordered_map<std::string, size_t> &frequency_map,
    std::priority_queue<data_structures::TreeNode*,
    std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap
){
    std::string buffer;

    //separator for utf - 8 strings
    for(uint8_t byte : file_content){
        bool is_runon = ((byte & 0xC0) == 0x80);

        if(is_runon){
            buffer.push_back(byte);
        }else{
            if(buffer.empty()){
                buffer.push_back(byte);
            }else{
                frequency_map[buffer]++;
                buffer.clear();
                buffer.push_back(byte);
            }
        }
    }

    if(!buffer.empty()){
        frequency_map[buffer]++;
    }

}

//huffman tree generated using node minheap
data_structures::TreeNode * HuffmanCompression::constructHuffmanTree(
    std::priority_queue<data_structures::TreeNode*,
    std::vector<data_structures::TreeNode*>,
    data_structures::TreeNode::compare> &node_minheap
){
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

//create decode map which allows for decompression
void HuffmanCompression::populateBinaryMap(
    data_structures::TreeNode * huffman_tree,
    std::unordered_map<std::string, std::string> &huffman_map,
    std::string &code_str
){
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
bool HuffmanCompression::comparator::operator()(
    const std::pair<std::string, size_t> &left,
    const std::pair<std::string, size_t> &right
) {

    if(left.second == right.second){
        return left.first.length() < right.first.length();
    }

    return left.second < right.second;
}


//used to encode plain text to huffman coded binary
std::string HuffmanCompression::encodeContent(
    std::string &content,
    std::unordered_map<std::string, std::string> &huffman_binary_map
){
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

    return encoded_str;
}

//encode content with a minum value allowed in memory at a time
void HuffmanCompression::buffEncodeContent(
    std::ifstream &file, std::ofstream &out_file,
    std::string &binary_code,
    std::unordered_map<std::string,
    std::string> &huffman_binary_map
){

    std::string main_buffer;
    std::string sub_buffer;

    char buff[util::CHBUF_SIZ];

    while(file.read(buff, util::CHBUF_SIZ)){

        if(!sub_buffer.empty()){//flush reminant buffer to actual content
            main_buffer.append(sub_buffer);
            sub_buffer.clear();
        }

        int ptr = util::CHBUF_SIZ - 1;

        //move pointer to start of character byte seq;
        while((static_cast<uint8_t>(buff[ptr]) & 0xC0) == 0x80){ // 10xxyyyy & 11000000
            //last character is a sequence;
            ptr--;
        }

        int norm_byte_count = util::getByteCount(static_cast<uint8_t>(buff[ptr]));

        if(norm_byte_count == 0){//change
            std::cerr << "invalid first byte found" << '\n';
            break;
        }

        int act_byte_count = (util::CHBUF_SIZ) - ptr;// actual amount of elements in sequence including start

        for(int i = 0; i < ptr; i++){//portion before remainder
            main_buffer.push_back(buff[i]);
        }

        if(act_byte_count < norm_byte_count){
            for(int i = ptr; i < util::CHBUF_SIZ; i++){
                sub_buffer.push_back(buff[i]);
            }
        }

        std::string encoded_content = encodeContent(main_buffer, huffman_binary_map);
        writeChar(encoded_content, out_file);

        main_buffer.clear();//clear buff
    }

    //flush reminants and buffer
    main_buffer.append(sub_buffer);

    for(int i = 0; i < file.gcount(); i++){
        main_buffer.push_back(buff[i]);
    }

    std::string encoded_content = encodeContent(main_buffer, huffman_binary_map);
    writeChar(encoded_content, out_file);
}

void HuffmanCompression::attachHeader(
    std::ofstream &out_file,
    std::unordered_map<std::string,
    std::string> &huffman_binary_map
){
    std::string binary_code;

    //flatten map (dictionary) to put in file
    std::string flat_map;
    int bit_len = 32;

    int t_size = 0;

    for(const auto &pair : huffman_binary_map){

        //convert string to binary rep;
        std::string char_rep;
        for(char c : pair.first){
            t_size++;
            std::bitset<8> ch_set(c);
            char_rep.append(ch_set.to_string());
        }

        flat_map.append(char_rep);

        //size of code
        std::bitset<32> len_set(pair.second.length());
        t_size += 4;

        flat_map.append(len_set.to_string());

        //add padding zeros to fit 8 bit
        std::string code(pair.second);
        while(code.length()%8 != 0){//CHANGEEEE
            code.append("0");
        }

        flat_map.append(code);
        t_size += (code.length()/8);
    }

    std::bitset<32> size_set(huffman_binary_map.size());
    t_size += 4;


    binary_code.append(size_set.to_string());

    binary_code.append(flat_map);

    writeChar(binary_code, out_file);
}

void HuffmanCompression::buffPopulateNodeMinHeap(
    std::ifstream &file,
    std::priority_queue<data_structures::TreeNode*,
    std::vector<data_structures::TreeNode*>,
    data_structures::TreeNode::compare> &node_minheap
){
    std::string main_buffer;
    std::string sub_buffer;

    std::unordered_map<std::string, size_t> frequency_map;

    char buff[util::CHBUF_SIZ];

    while(file.read(buff, util::CHBUF_SIZ)){

        if(!sub_buffer.empty()){//flush reminant buffer to actual content
            main_buffer.append(sub_buffer);
            sub_buffer.clear();
        }

        int ptr = util::CHBUF_SIZ - 1;

        //move pointer to start of character byte seq;
        while((static_cast<uint8_t>(buff[ptr]) & 0xC0) == 0x80){ // 10xxyyyy & 11000000
            //last character is a sequence;
            ptr--;
        }

        int norm_byte_count = util::getByteCount(static_cast<uint8_t>(buff[ptr]));

        if(norm_byte_count == 0){//
            std::cerr << "unrecognized character found" << '\n';
            break;
        }

        int act_byte_count = (util::CHBUF_SIZ) - ptr;// actual amount of elements in sequence including start

        for(int i = 0; i < ptr; i++){//portion before remainder
            main_buffer.push_back(buff[i]);
        }

        if(act_byte_count < norm_byte_count){
            for(int i = ptr; i < util::CHBUF_SIZ; i++){
                sub_buffer.push_back(buff[i]);
            }
        }

        populateNodeMinHeap(main_buffer, frequency_map, node_minheap);
        main_buffer.clear();//clear buff
    }

    //flush reminants and buffer
    main_buffer.append(sub_buffer);

    for(int i = 0; i < file.gcount(); i++){
        main_buffer.push_back(buff[i]);
    }

    populateNodeMinHeap(main_buffer, frequency_map, node_minheap);

    //added
    for(const auto &map_pair : frequency_map){
        data_structures::TreeNode * node = new data_structures::TreeNode(map_pair.second, map_pair.first);
        node_minheap.push(node);
    }
}




void HuffmanCompression::writeChar(
    std::string &bits,
    std::ofstream &binary_file
){
    std::string buffer;// accumilate string bit by bit;
    for(const char c : bits){
        if(buffer.size() == 8){
            uint8_t byte = util::convertChunkToByte(buffer);

            binary_file.put(byte);

            buffer.clear();
        }

        buffer.push_back(c);

    }

    if(!buffer.empty()){//pad with zeros if not empty so
        while (buffer.size() < 8) {
            buffer.push_back('0');
        }

        uint8_t byte = util::convertChunkToByte(buffer);

        std::bitset<8> set(byte);

        binary_file.put(byte);

        buffer.clear();
    }
}
