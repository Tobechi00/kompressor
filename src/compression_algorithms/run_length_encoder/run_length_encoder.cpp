#include "run_length_encoder.h"
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>


RunLengthEncoder::RunLengthEncoder(std::string &binary_str){
    if(binary_str.length() == 0){
        return;
    }

    encodeStr(binary_str, this->length_bit_pair);
}

void RunLengthEncoder::encodeStr(std::string &binary_str, std::vector<std::pair<size_t, char>> &length_bit_pair){
    //store frequency and bit pairs, use largest frequency to set bit width standard

    std::string &rle_str = this->rle_str;

    size_t max_frequency = 0;

    char current = binary_str[0];
    size_t frequency = 0;

    for(char c : binary_str){

        if(c == current){
            frequency++;
        }else{
            max_frequency = std::max(max_frequency, frequency);

            std::pair<size_t, char> pair;

            pair.first = frequency;
            pair.second = current;
            length_bit_pair.push_back(pair);
            ////

            current = c;
            frequency = 1;
        }
    }

    std::pair<size_t, char> pair;

    pair.first = frequency;
    pair.second = current;

    length_bit_pair.push_back(pair);

    //get length of largest bit
    std::string bit_rep;//todo wrong constructor convert to base two

    convertBinary(bit_rep, max_frequency);

    generateRleStr(length_bit_pair, this -> rle_str, bit_rep.length());
}

//decide wether or not to encode based on size diff
void RunLengthEncoder::generateRleStr(std::vector<std::pair<size_t, char>> &length_bit_pair, std::string &rle_str, const size_t bit_width){

    for(const auto pair : length_bit_pair){

        //get num zeros to keep a standard bit width
        std::string temp_str;
        convertBinary(temp_str, pair.first);

        int str_length = temp_str.length();

        size_t needed_zeros = 0;

        if(bit_width > str_length){
            needed_zeros = bit_width - str_length;
        }

        std::string zeros(needed_zeros, pair.second);

        temp_str = zeros + temp_str;
        rle_str.append(temp_str).push_back(pair.second);
    }
}

std::string& RunLengthEncoder::getRleString(){
    return this -> rle_str;
}

void RunLengthEncoder::convertBinary(std::string &str, size_t num){

    if(num == 0){
        str.push_back(0);
        return;
    }

   	while(num != 0){

	    if(num == 1){
	        num = 0;
	        str.push_back('1');
	        break;
	    }
	    size_t val = num / 2;

	    str.push_back('0' + (num % 2));
	    num = val;
	}
}
