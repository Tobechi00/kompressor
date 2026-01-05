#ifndef KEY_SCHEDULER_H
#define KEY_SCHEDULER_H


#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>
class KeyScheduler{
    std::vector<std::vector<uint8_t>> expanded_key;
    std::vector<std::vector<uint8_t>> original_key;
    uint8_t rcon_val;

    public:
    KeyScheduler(std::string user_key, int ver);

    private:
    void rot_word(std::vector<uint8_t> &word);

    void sub_word(std::vector<uint8_t> &word);
    void op_rcon(std::vector<uint8_t> &word);

    std::vector<std::vector<uint8_t>> get_expanded_key();
};


#endif
