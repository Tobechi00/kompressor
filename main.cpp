#include <iostream>
#include <string>
#include <unordered_map>
#include "compression_algorithms/huffman_compression/huffman_compression.h"
#include "file_utils/file_utils.h"


int main(int argc, char* argv[]){

    //todo set up logic for commandline
    //arg[1] - command
    //arg[2] - filepath

    std::string file_content;

    try {
        FileUtils file_util(argv[2]);

        HuffmanCompression huffman_compression(file_util.getFileContent());

        std::unordered_map<char, std::string> map = huffman_compression.getHuffmanDecodeMap();

        for(auto &pair : map){
            std::cout << pair.first <<" -- " << pair.second << "\n";
        }
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

}
