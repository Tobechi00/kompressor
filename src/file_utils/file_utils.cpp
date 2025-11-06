#include "file_utils.h"
#include <climits>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

FileUtils::FileUtils(const std::string &file_directory){

    try {
        readFileContent(file_directory);
        this -> is_valid = true;
    }catch (std::exception &e) {
        throw;
    }

    //file read successfully

    if(this -> is_valid == true){
        this -> file_path = std::filesystem::path(file_directory);
    }
}

const std::string& FileUtils::getFileContent(){
    if(this -> is_valid){
        return file_content;
    }else{
        throw std::runtime_error("could not get file content as file could not be processed \n");
    }
}

void FileUtils::readFileContent(const std::string &directory) noexcept(false){

    std::cout << "reading file..." << "\n";

    raw_file.open(directory, std::ios::out);

    if(!raw_file.is_open()){
        throw std::runtime_error("could not open file with name: "+directory+" \n");
    }

    std::ostringstream stream;

    stream << raw_file.rdbuf();
    file_content = stream.str();

    raw_file.close();

    std::cout << "file read \n";
}

void FileUtils::createFile(const std::string &content){//change
    std::ofstream file(this -> file_path.replace_extension(".txt"), std::ios::trunc);

    if(file.is_open()){
        file << content;
    }else{
        std::cout << "unable to create file \n";
    }

    file.close();
}
void FileUtils::createCompressedFile(const std::string &bits){ //todo: add dictionary

    std::ofstream binary_file(this -> file_path.replace_extension(".kmp"), std::ios::binary | std::ios::trunc);

    if(binary_file.is_open()){

        std::string buffer;// accumilate string bit by bit;

        for(const char c : bits){
            if(buffer.size() == 8){
                uint8_t byte = convertChunkToByte(buffer);
                binary_file.put(byte);

                buffer.clear();
                buffer.push_back(c);
            }else{
                buffer.push_back(c);
            }
        }

        if(!buffer.empty()){//pad with zeros if not empty
            while (buffer.size() < 8) {
                buffer.push_back('0');
            }

            uint8_t byte = convertChunkToByte(buffer);
            binary_file.put(byte);

            buffer.clear();
        }

        binary_file.close();
    }else{
        throw std::runtime_error("an error occurred while creating file \n");
    }
}

uint8_t FileUtils::convertChunkToByte(const std::string &chunk){//reads 8 bit chunk

    int num_rep = 0;//base 10 representation

    for(int i = 7; i >= 0; i--){//converting binary_str to base 10
        num_rep += (chunk[i] - '0') * std::pow(2, i);
    }

    uint8_t byte = static_cast<uint8_t>(num_rep);

    return byte;
}
