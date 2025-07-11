#include "file_utils.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>

FileUtils::FileUtils(const std::string &n_file_directory){
    this -> file_directory = n_file_directory;

    std::string n_file_name;

    for(int i = file_directory.length() - 1; i >= 0; i--){
        char current_character = file_directory.at(i);

        if(current_character == '/' || current_character == '\\'){
            break;
        }

        n_file_name.push_back(current_character);
    }

    std::reverse(n_file_name.begin(), n_file_name.end());

    this -> file_name = n_file_name;

    try {
        readFileContent(file_directory);
    }catch (std::exception &e) {
        throw;
    }

    is_read = true; //file successfully read
}

const std::string& FileUtils::getFileContent(){
    if(is_read){
        return file_content;
    }else{
        throw std::runtime_error("could not get file content as file could not be processed");
    }
}

void FileUtils::readFileContent(const std::string &directory) noexcept(false){
    raw_file.open(directory, std::ios::out); //open file for output REMEMBER TO CLOSE

    if(!raw_file.is_open()){
        std::runtime_error("could not open file with name: "+directory);
    }

    std::ostringstream stream;

    stream << raw_file.rdbuf();
    file_content = stream.str();

    raw_file.close();
}
