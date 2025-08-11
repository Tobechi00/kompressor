#pragma once

#include <cstddef>
#include <string>
#include <sys/types.h>
#include <unordered_map>
class HuffmanDecompression{
    std::string decompressed_text;

    public:
    HuffmanDecompression(const std::string &file_content);

    std::string& getDecompressedText();

    private:
    std::unordered_map<std::string, std::string>& getMap();

    size_t convertBinaryToDecimal(std::string &binary_str);

    int getCharByteLength(std::string first_byte);

    std::string extractCode(const std::string &file_content, int code_len, int pos);

    int populateDictionary(std::unordered_map<std::string, std::string> &str_code_map, const std::string &file_content);

    std::string convertBigEndian(u_int8_t byte);

    char byteToChar(u_int8_t byte);
};
