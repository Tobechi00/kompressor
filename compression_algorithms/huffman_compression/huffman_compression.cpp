#include "huffman_compression.h"
#include "data_structures/tree/tree.h"



HuffmanCompression::HuffmanCompression(const std::string &n_file_content){

    populateNodeMinHeap(n_file_content, node_minheap);

    if(node_minheap.size() == 1){//edge case where we only have one element in our heap //(i.e only one unique element)

        data_structures::TreeNode * only_node = node_minheap.top();

        char only_char = only_node-> value;

        huffman_decode_map[only_char] = 1; //assign single bit code to only char;

        node_minheap.pop();

        delete only_node;
        only_node = nullptr;
    }else{
        data_structures::TreeNode * huffman_tree = &constructHuffmanTree(node_minheap);
        std::string huffman_code;

        populateDecodeMap(huffman_tree, huffman_decode_map, huffman_code);
    }

}


/*
 * add values to min heap
 */
void HuffmanCompression::populateNodeMinHeap(const std::string &file_content, std::priority_queue<data_structures::TreeNode*, std::vector<data_structures::TreeNode*>, data_structures::TreeNode::compare> &node_minheap){
    std::unordered_map<char, int> frequency_map;

    for(char c : file_content){//generate frequency map
        if(frequency_map.find(c) != frequency_map.end()){
            frequency_map[c]++;
        }else{
            frequency_map[c] = 1;
        }
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
void HuffmanCompression::populateDecodeMap(data_structures::TreeNode * huffman_tree, std::unordered_map<char, std::string> &huffman_map, std::string &huffman_code){
    if(huffman_tree -> left == nullptr && huffman_tree -> right == nullptr){
        huffman_map[huffman_tree -> value] =  huffman_code;
        return;
    }

    //left == 0, right == 1 -- bit pattern

    huffman_code.push_back('1');
    populateDecodeMap(huffman_tree -> right, huffman_map, huffman_code);
    huffman_code.pop_back(); //backtrack

    huffman_code.push_back('0');
    populateDecodeMap(huffman_tree -> left, huffman_map, huffman_code);
    huffman_code.pop_back();
}
