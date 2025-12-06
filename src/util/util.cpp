#include "src/util/util.h"
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>

namespace util{

    const int CHBUF_SIZ = 8000;

    uint8_t convertChunkToByte(const std::string &chunk){
        int ans = 0;

        for(int i = 0; i < 8; i++){
            int bit = chunk[i] - '0';
            ans += std::pow(2, 7 - i) * bit;
        }

        return static_cast<uint8_t>(ans);
    }

    std::string generateCompressionOut(std::string &original_path){
        std::filesystem::path file_path(original_path);
        file_path.replace_extension(".kmp");

        return file_path;
    }

    std::string generateDecompressionOut(std::string &original_path){
        std::filesystem::path file_path(original_path);
        file_path.replace_extension(".txt");

        return file_path;
    }

    int getByteCount(uint8_t f_char){

        if((f_char & 0x80) == 0x00){//one byte
            return 1;
        }else if((f_char & 0xE0) == 0xC0){//two bytes
            return 2;
        }else if((f_char & 0xF0) == 0xE0){//three bytes
            return 3;
        }else if((f_char & 0xF8) == 0xF0){//four bytes
            return 4;
        }else{//unrecognized
            return 0;
        }
    }

    int getNthBit(uint8_t byte, int n){
        int bit_val = (byte >> n) & 0x1;
        return bit_val == 0x1 ? 1 : 0;
    }

    uint32_t read_32bits(std::ifstream &file){////////

        uint32_t bits = 0x0000;
        char buff[4];

        file.read(buff, 4);

        bits = bits | (static_cast<uint32_t>(buff[0]) << 24)
            |(static_cast<uint32_t>(buff[1]) << 16)
            |(static_cast<uint32_t>(buff[2]) << 8)
            |static_cast<uint32_t>(buff[3]);


        return bits;
    }

}
