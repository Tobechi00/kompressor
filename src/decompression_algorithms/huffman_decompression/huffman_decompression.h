#ifndef _HUFFMAN_DECOMPRESSION_H
#define _HUFFMAN_DECOMPRESSION_H

#include <fstream>
#include <string>
#include <unordered_map>
class HuffmanDecompression{

    public:
    HuffmanDecompression(std::string &file_path);
    std::string& getDecompressedText();

    private:
    std::unordered_map<std::string, std::string>& getMap();

    void populateDictionary(
        std::unordered_map<std::string, std::string> &str_code_map,
        std::ifstream &file
    );

    void decodeText(
        std::ifstream &compressed_file,
        std::ofstream &decompressed_file,
        std::unordered_map<std::string, std::string> &dictionary
    );

};

#endif
