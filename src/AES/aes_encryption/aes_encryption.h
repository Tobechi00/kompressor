#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H

#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>
class AesEncryption{

    static const std::vector<int> poly_rs_8;//4
    static const std::vector<std::vector<uint8_t>> galois_field;//4x4

    public:
    AesEncryption(std::string file_name, std::string user_key);

    private:
    uint8_t mix_col(uint8_t field_val, uint8_t state_val);

    uint8_t rot_word(uint8_t val);

};

#endif
