#include "commands.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <unordered_set>
#include "src/decompression_algorithms/huffman_decompression/huffman_decompression.h"
#include "src/file_utils/file_utils.h"
#include "src/compression_algorithms/huffman_compression/huffman_compression.h"

/* current command list
 * --compress   | -c
 * --decompress | -d
 * --help       | -h
 */


void Commands::parseCommand(int command_len, char *command_array[]){

    if(command_len == 2){
        std::string first_command(command_array[1]);
        executeCommand(first_command);
    }else if(command_len == 3){
        std::string first_command(command_array[1]);
        std::string second_command(command_array[2]);
        executeCommand(first_command, second_command);
    }else{//no available commands shorter than 2 or longer than 3
        std::cerr << "unrecognised command. See \'kompressor --help\'\n";
    }
}

void Commands::executeCommand(std::string &command, std::string &value){

    std::filesystem::path file_path = std::filesystem::path(value);

    std::unordered_set<std::string> compressables = {".txt", ".word"," ", ".avif"};
    std::unordered_set<std::string> decompressables = {".kmp"};

    if(command == "-c" || command == "--compress" ){

        std::string extension = file_path.extension();

        if(compressables.find(extension) == compressables.end() && !extension.empty()){//FIXXXXX
            std::cerr << "inputed file is not a text file \n";
            return;
        }

        try {
            FileUtils file_util(value);

            HuffmanCompression huffman_compression(file_util.getFileContent());

            file_util.createCompressedFile(huffman_compression.getBinaryCode());

        } catch (std::exception &e) {
            std::cerr << e.what();
        }
    }else if(command == "-d" || command == "--decompress"){

        if(decompressables.find(file_path.extension()) == decompressables.end()){
            std::cerr << "inputed file is not a kmp file \n";
            return;
        }

        FileUtils file_util(value);
        HuffmanDecompression huffman_decompression(file_util.getFileContent());
        file_util.createFile(huffman_decompression.getDecompressedText());
    }else{
        std::cerr << "kompressor: \"" << command << "\" is not a kompressor command. See \'kompressor --help\'\n";
    }
}

//overload for solo commands
void Commands::executeCommand(std::string &command){
    if(command == "-h" || command == "--help"){
        displayHelpMenu();
    }else{
        std::cerr << "unrecognised command. See \'kompressor --help\'\n";
    }
}

void Commands::displayHelpMenu(){
    std::cout << "usage: kompressor [\'-h\' | \'--help\'], [\'-c\' | \'--compress\'], [\'-d\' | \'--decompress\']\n";
}
