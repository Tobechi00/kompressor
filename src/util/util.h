#ifndef UTIL_H
#define UTIL_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace util{

    const extern int CHBUF_SIZ;

    //byte operations
    extern uint8_t convertChunkToByte(const std::string &chunk);
    extern int getByteCount(uint8_t f_char);
    extern uint32_t read_32bits(std::ifstream &file);
    extern int getNthBit(uint8_t byte, int n);

    extern size_t getFileSize(std::ifstream &file);
    extern size_t getFileSize(std::ofstream &file);

    //string operations
    extern std::string generateCompressionOut(std::string &original_path);
    extern std::string generateDecompressionOut(std::string &original_path);
}

#endif
