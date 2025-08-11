#include "commands.h"
#include <string>
#include <iostream>
#include "decompression_algorithms/huffman_decompression/huffman_decompression.h"
#include "file_utils/file_utils.h"
#include "compression_algorithms/huffman_compression/huffman_compression.h"

namespace commands{
    void parseCommand(int command_amount, char *command_array[]){

        if(command_amount == 1){
            commands::displayHelpMenu();
        }else if(command_amount == 2){
            std::string first_command = command_array[1];
            commands::executeCommand(first_command);
        }else if(command_amount == 3){
            std::string first_command = command_array[1];
            std::string second_command = command_array[2];
            commands::executeCommand(first_command, second_command);
        }else{
            std::cerr << "unrecognised command. See \'kompressor --help\'\n";
        }
    }

    //temporary
    void executeCommand(std::string &command, std::string &value){

        if(command == "-c" || command == "--compress"){ //value can only be a string, attempt to compress

            try {
                FileUtils file_util(value);

                HuffmanCompression huffman_compression(file_util.getFileContent());

                file_util.createCompressedFile(huffman_compression.getBinaryCode());

            } catch (std::exception &e) {
                std::cerr << e.what();
            }
        }else if(command == "-d" || command == "--decompress"){
            FileUtils file_util(value);
            HuffmanDecompression huffman_decompression(file_util.getFileContent());


            // for(const auto pair : huffman_decompression.getMap()){
            //     std::cout << pair.first +"  "+pair.second << "\n";
            // }
        }else{
            std::cerr << "kompressor: \"" << command << "\" is not a kompressor command. See \'kompressor --help\'\n";
        }
    }

    void executeCommand(std::string &command){
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
