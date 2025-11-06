#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
class HuffmanDecompression{
    private:
    std::string decompressed_text;

    public:
    HuffmanDecompression(const std::string &file_content);

    std::string& getDecompressedText();

    private:
    std::unordered_map<std::string, std::string>& getMap();

    size_t convertBinaryToDecimal(std::string &binary_str);

    int getCharByteLength(std::string first_byte);

    std::string extractCode(const std::string &file_content, int code_len, int pos);

    size_t populateDictionary(std::unordered_map<std::string, std::string> &str_code_map, const std::string &file_content);

    std::string convertBigEndian(uint8_t byte);

    char byteToChar(uint8_t byte);
};
