#include "commands.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <unordered_set>
#include "decompression_algorithms/huffman_decompression/huffman_decompression.h"
#include "file_utils/file_utils.h"
#include "compression_algorithms/huffman_compression/huffman_compression.h"


void Commands::parseCommand(int command_len, char *command_array[]){

    if(command_len == 1){
        displayHelpMenu();
    }else if(command_len == 2){
        std::string first_command(command_array[1]);
        executeCommand(first_command);
    }else if(command_len == 3){
        std::string first_command(command_array[1]);
        std::string second_command(command_array[2]);
        executeCommand(first_command, second_command);
    }else{
        std::cerr << "unrecognised command. See \'kompressor --help\'\n";
    }
}

    //temporary
void Commands::executeCommand(std::string &command, std::string &value){

    std::filesystem::path file_path = std::filesystem::path(value);

    std::unordered_set<std::string> compressables = {".txt", ".word"," ", ".avif"};
    std::unordered_set<std::string> decompressables = {".kmp"};

    if(command == "-c" || command == "--compress"){ //value can only be a string, attempt to compress

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


        // for(const auto pair : huffman_decompression.getMap()){
        //     std::cout << pair.first +"  "+pair.second << "\n";
        // }
    }else{
        std::cerr << "kompressor: \"" << command << "\" is not a kompressor command. See \'kompressor --help\'\n";
    }
}

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
