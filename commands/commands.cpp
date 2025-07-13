#include "commands.h"
#include <string>
#include <iostream>
#include "file_utils/file_utils.h"
#include "compression_algorithms/huffman_compression/huffman_compression.h"

namespace commands{

    //temporary
    void parseCommand(std::string &command, std::string &value){

        if(command == "-c" || command == "--compress"){ //value can only be a string, attempt to compress

            try {
                FileUtils file_util(value);

                HuffmanCompression huffman_compression(file_util.getFileContent());

                std::unordered_map<char, std::string> map = huffman_compression.getHuffmanDecodeMap();

                for(auto &pair : map){
                    std::cout << pair.first <<" -- " << pair.second << "\n";
                }
            } catch (std::exception &e) {
                std::cerr << e.what();
            }
        }else if(command == "-d" || command == "--decompress"){
            //todo
        }else{
            std::cerr << "kompressor: \"" << command << "\" is not a kompressor command. See \'kompressor --help\'\n";
        }
    }

    void parseCommand(std::string &command){
        if(command == "-h" || command == "--help"){
            displayHelpMenu();
        }else{
            std::cerr << "unrecognised command. See \'kompressor --help\'\n";
        }
    }

    void displayHelpMenu(){
        std::cout << "usage: kompressor [\'-h\' | \'--help\'], [\'-c\' | \'--compress\'], [\'-d\' | \'--decompress\']\n";
    }
}
