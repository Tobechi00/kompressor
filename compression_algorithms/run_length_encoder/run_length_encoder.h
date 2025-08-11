#pragma once

#include <cstddef>
#include<string>
#include<vector>

//unused class for now, ignore ;)
class RunLengthEncoder{

    std::string rle_str;
    std::vector<std::pair<size_t, char>> length_bit_pair;

    public:
    RunLengthEncoder(std::string &binary_str);

    private:
    void encodeStr(std::string &binary_str, std::vector<std::pair<size_t, char>> &length_bit_pair);
    void generateRleStr(std::vector<std::pair<size_t, char>> &length_bit_pair, std::string &rle_str, size_t bit_width);
    void convertBinary(std::string &str, size_t num);

    public:
    std::string& getRleString();
};
