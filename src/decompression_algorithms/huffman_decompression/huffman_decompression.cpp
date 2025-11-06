#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include "huffman_decompression.h"

#define BYTE_SIZE 8

//#define byte uint8_t use this

HuffmanDecompression::HuffmanDecompression(const std::string &file_content){

    //decompression style
    // first 4 bytes = decode map length
    // next decode map is extracted
    // finally file content

    std::unordered_map<std::string, std::string> code_map;

    size_t code_start_pos = populateDictionary(code_map, file_content);

    std::string converted_code;

    for(int i = code_start_pos; i < file_content.size(); i++){
        converted_code.append(convertBigEndian(file_content[i]));
    }

    std::string final_str;
    std::string buffer;

    for(char c : converted_code){
        buffer.push_back(c);
        if(code_map.find(buffer) != code_map.end()){
            final_str.append(code_map[buffer]);
            buffer.clear();
        }
    }

    this -> decompressed_text = final_str;

    size_t original_len = file_content.length();
    size_t new_len = final_str.length();


    std::cout <<"file decompressed \n";
}

int HuffmanDecompression::getCharByteLength(std::string first_byte){
    int byte_len;

    if(first_byte[0] == '0'){//1 byte
        byte_len = 1;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '0'){//2 bytes
        byte_len = 2;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '1' && first_byte[3] == '0'){
        byte_len = 3;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '1' && first_byte[3] == '1' && first_byte[4] == '0'){
        byte_len = 4;
    }

    return byte_len;
}

size_t HuffmanDecompression::convertBinaryToDecimal(std::string &binary_string){
    size_t ans = 0;
    size_t exponent = binary_string.length() - 1;

    for(size_t i = 0; i < binary_string.length(); i++){
        int bit = binary_string[i] - '0';
        ans += bit * std::pow(2, exponent);

        exponent--;
    }

    return ans;
}



//when bytes are read they are read with the least significant bit first
//because of this we get an invalid char, so we reverse to make it big endian
std::string HuffmanDecompression::convertBigEndian(uint8_t byte){
    std::bitset<BYTE_SIZE> bit_rep(byte);

    std::string val = bit_rep.to_string();
    std::reverse(val.begin(), val.end());

    return val;
}

char HuffmanDecompression::byteToChar(uint8_t byte){
    std::string temp = convertBigEndian(byte);
    char char_val =  static_cast<char>(convertBinaryToDecimal(temp));

    return char_val;
}

//populates dictionary and returns position of code start
size_t HuffmanDecompression::populateDictionary(std::unordered_map<std::string, std::string> &code_map, const std::string &file_content){
    size_t content_ptr = 0;
    std::string len_str;


    for(int i = 0; i < 4; i++){//extract map length stored in first four bytes
        len_str.append(convertBigEndian(file_content[content_ptr]));
        content_ptr++;
    }

    size_t map_len = convertBinaryToDecimal(len_str);

    for(size_t i = 0; i < map_len; i++){

        std::string first_byte = convertBigEndian(file_content[content_ptr]);
        int num_bytes = getCharByteLength(first_byte) - 1;//already have first byte so reduce by one
        std::string key;

        key.push_back(byteToChar(file_content[content_ptr]));

        for(int j = 0; j < num_bytes; j++){
            content_ptr++;
            key.push_back(byteToChar(file_content[content_ptr]));
        }

        content_ptr++;//move to next byte
        std::string code_len_str = convertBigEndian(file_content[content_ptr]);
        int code_len = convertBinaryToDecimal(code_len_str);

        std::string code_str;
        for(int i = 0; i < 4; i++){
            content_ptr++;
            code_str.append(convertBigEndian(file_content[content_ptr]));
        }

        code_str = code_str.substr(0, code_len);
        content_ptr++;//move to next starting point

        code_map[code_str] = key;
    }

    return content_ptr;
}


std::string& HuffmanDecompression::getDecompressedText(){
    return this->decompressed_text;
}
