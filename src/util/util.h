#ifndef _UTIL_H
#define _UTIL_H

#include <cstdint>
#include <string>

namespace util{

    const extern int CHBUF_SIZ;

    //byte operations
    extern uint8_t convertChunkToByte(const std::string &chunk);
    extern int getByteCount(uint8_t f_char);
    extern uint32_t read_32bits(std::ifstream &file);
    extern int getNthBit(uint8_t byte, int n);

    //string operations
    extern std::string generateCompressionOut(std::string &original_path);
    extern std::string generateDecompressionOut(std::string &original_path);
}

#endif
