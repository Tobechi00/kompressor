#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include "huffman_decompression.h"
#include "src/util/util.h"


HuffmanDecompression::HuffmanDecompression( std::string &file_path){


    std::ifstream compressed_file(file_path, std::ios::binary);

    std::ofstream decompressed_file(util::generateDecompressionOut(file_path));

    if(!compressed_file.is_open()){
        std::cerr << "Unable to open file" << "\n";
        return;
    }

    if(!decompressed_file.is_open()){
        std::cerr << "Unable to write to output file" << "\n";
        return;
    }

    std::unordered_map<std::string, std::string> code_map;

    populateDictionary(code_map, compressed_file);

    decodeText(compressed_file, decompressed_file, code_map);

    size_t new_size = util::getFileSize(decompressed_file);

    std::cout << "approx file size post decompression: "<< new_size << "KB \n";

    compressed_file.close();
    decompressed_file.close();
}


void HuffmanDecompression::populateDictionary(
    std::unordered_map<std::string, std::string> &code_map,
    std::ifstream &compressed_file
){

    size_t dict_size = static_cast<int>(util::read_32bits(compressed_file));//read first 4 bytes


    while(dict_size > 0){

        std::string char_rep;

        uint8_t f_char = compressed_file.get();//get first char;
        char_rep.push_back(f_char);

        int byte_count = util::getByteCount(f_char) - 1;//read byte and code length

        if(byte_count > 1){
            char rem_chars[byte_count];
            compressed_file.read(rem_chars, byte_count);

            if(compressed_file.fail()){
                std::cerr << "an error occured while reading file";
                return;
            }

            for(int i = 0; i < byte_count; i++){
                char_rep.push_back(rem_chars[i]);
            }
        }

        size_t code_len = static_cast<size_t>(util::read_32bits(compressed_file));

        size_t req_byte = code_len + 7;
        req_byte = (req_byte & ~7)/8;

        char code_arr[req_byte];
        compressed_file.read(code_arr, req_byte);


        std::string code;

        for(int i = 0; i < req_byte; i++){
            std::bitset<8> set(code_arr[i]);
            code.append(set.to_string());
        }

        code = code.substr(0, code_len);//reduce to actual length

        code_map[code] = char_rep;
        dict_size--;
    }
}

void HuffmanDecompression::decodeText(
    std::ifstream &compressed_file,
    std::ofstream &decompressed_file,
    std::unordered_map<std::string,
    std::string> &dictionary
){

    char buffer[util::CHBUF_SIZ];

    std::string text;
    std::string rem_chars;

    while(!compressed_file.eof()){

    compressed_file.read(buffer, util::CHBUF_SIZ);

    std::string utf8_char;

        if(!rem_chars.empty()){
            utf8_char.append(rem_chars);
            rem_chars.clear();
        }

        for(int i = 0; i < compressed_file.gcount(); i++){

            for(int j = 7; j >= 0; j--){
                int bit = util::getNthBit(buffer[i], j);
                utf8_char.append(std::to_string(bit));

                if(dictionary.find(utf8_char) != dictionary.end()){
                    text.append(dictionary[utf8_char]);
                    utf8_char.clear();
                }
            }
        }

        //write text to file
        decompressed_file << text;

        text.clear();

        if(!utf8_char.empty()){
            rem_chars.append(utf8_char);
        }
    }

}
