#include <iostream>
#include <string>
#include <unordered_map>
#include "compression_algorithms/huffman_compression/huffman_compression.h"
#include "file_utils/file_utils.h"


//spin on huffman convert all the text to small letters for less characters and then store them with a queue/list where each value is a 1 or 0
//if its a one then its capital if not then its small eg map: code -- 'Q' (char) -- <0110> this basically means that for each character
//that appears seeing as all the 'q's will be accessed inorder each will be properly recreated
// struct TreeNode{// tree node def

//     int frequency;
//     char value;



//     TreeNode * left = nullptr;
//     TreeNode * right = nullptr;

//     TreeNode(){}
//     TreeNode(int n_frequency): frequency(n_frequency){};
//     TreeNode(int n_frequency, char n_value): frequency(n_frequency), value(n_value){};

//     struct compare{ // comparator for priority queue
//         bool operator()(const TreeNode * node_a, const TreeNode * node_b){
//             return node_a -> frequency > node_b -> frequency;
//         }
//     };
// };


// void populateNodeMinHeap(const std::string&, std::priority_queue<datastructures::TreeNode*, std::vector<datastructures::TreeNode*>, datastructures::TreeNode::compare>&);
// bool naturalOrderComparator(const std::pair<char, int>&, const std::pair<char, int>&);
// datastructures::TreeNode &constructHuffmanTree(std::priority_queue<datastructures::TreeNode*, std::vector<datastructures::TreeNode*>, datastructures::TreeNode::compare> &node_minheap);
// void populateDecodeMap(datastructures::TreeNode *, std::unordered_map<char, std::string>&, std::string&);

int main(int argc, char* argv[]){


    //accept only txt files for read
    //error checkers

    //todo- os checker
    //convert utf8 encoded file to utf 32

    // FileUtils file_util("file dir");
    // HuffmanCompression huffman_compressor();
    //
    std::string file_content;

    try {
        FileUtils file_util("/home/tobechi/test_text/document");

        HuffmanCompression huffman_compression(file_util.getFileContent());

        std::unordered_map<char, std::string> map = huffman_compression.getHuffmanDecodeMap();

        // for(auto &pair : map){
        //     std::cout << pair.first <<" -- " << pair.second << "\n";
        // }

        std::cout << file_content;
    } catch (std::exception &e) {
        std::cout << e.what();
    }


    // if(file_util.getFileContent().length() == 0){
    //     //preform operation for empty file;
    // }
}


// void populateNodeMinHeap(const std::string &file_content, std::priority_queue<datastructures::TreeNode*, std::vector<datastructures::TreeNode*>, datastructures::TreeNode::compare> &node_minheap){
//     std::unordered_map<char, int> frequency_map;

//     for(char c : file_content){//generate frequency map
//         if(frequency_map.find(c) != frequency_map.end()){
//             frequency_map[c]++;
//         }else{
//             frequency_map[c] = 1;
//         }
//     }

//     for(const auto &map_pair : frequency_map){//loop through address
//         datastructures::TreeNode * node = new datastructures::TreeNode(map_pair.second, map_pair.first);

//         node_minheap.push(node);
//     }
// }


// datastructures::TreeNode& constructHuffmanTree(std::priority_queue<datastructures::TreeNode*, std::vector<datastructures::TreeNode*>, datastructures::TreeNode::compare> &node_minheap){

//     while(node_minheap.size() > 1){
//         datastructures::TreeNode * combination_node;

//         datastructures::TreeNode * first_node = node_minheap.top();
//         node_minheap.pop();//remove ptr from queue

//         datastructures::TreeNode * second_node = node_minheap.top();
//         node_minheap.pop();//remove ptr from queue

//         int sum_frequency = first_node -> frequency + second_node -> frequency;
//         combination_node = new datastructures::TreeNode(sum_frequency);//combine node frequency and create new nodesecon

//         combination_node -> left = first_node;
//         combination_node -> right = second_node; //assign left and right nodes to new tree;

//         node_minheap.push(combination_node);
//     }

//     return *node_minheap.top();
// }

// void populateDecodeMap(datastructures::TreeNode * huffman_tree, std::unordered_map<char, std::string> &huffman_map, std::string &huffman_code){
//     if(huffman_tree -> left == nullptr && huffman_tree -> right == nullptr){
//         huffman_map[huffman_tree -> value] =  huffman_code;
//         return;
//     }

//     //left == 0, right == 1 -- bit pattern

//     huffman_code.push_back('1');
//     populateDecodeMap(huffman_tree -> right, huffman_map, huffman_code);
//     huffman_code.pop_back(); //backtrack

//     huffman_code.push_back('0');
//     populateDecodeMap(huffman_tree -> left, huffman_map, huffman_code);
//     huffman_code.pop_back();
// }

// bool naturalOrderComparator(const std::pair<char, int> &pair_a, const std::pair<char, int> &pair_b){
//     return pair_a.second < pair_b.second;
// }
