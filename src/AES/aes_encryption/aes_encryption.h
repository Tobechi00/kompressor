#ifndef _AES_ENCRYPTION_H
#define _AES_ENCRYPTION_H

#include <cstdint>
#include <string>
#include <sys/types.h>
class aes_encryption{

    static const int poly_rs_8[4];
    static const uint8_t s_box[16][16];
    static const uint8_t galois_field[4][4];

    public:
    aes_encryption(std::string file_name, std::string user_key);

    private:
    uint8_t mix_col(uint8_t field_val, uint8_t state_val);

    uint8_t rot_word(uint8_t val);

};

#endif
