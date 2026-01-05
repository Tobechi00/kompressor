#include "key_scheduler.h"
#include "src/util/util.h"
#include <cstdint>
#include <sys/types.h>

KeyScheduler::KeyScheduler(std::string user_key, int aes_ver){
    this -> rcon_val = 0x01;

    int key_size = aes_ver/4;

    if(key_size <= 6){
        //arranged in column major order
        this -> original_key = this -> expanded_key = std::vector<std::vector<uint8_t>>(4);

        int ptr = 0; //(do depending on size) on each round (like each round divisible by key size
        //ie key 12 for aes 6 we perform all the core functions but for rounds that are not we just
        // perform key xor key ie k7 = k6 xor k1)
        // https://crypto.stackexchange.com/questions/51951/aes-key-expansion-for-192-bit

        for(int col = 0; col < key_size; col++){

            for(int row = 0; row < 4; row++){
                original_key[row].push_back(user_key[ptr]);

                ptr++;
            }
        }

        int curr_num_col = key_size;
        int req_num_col = 0;

        if(key_size == 4){
            req_num_col = 44;
        }else{
            req_num_col = 52;
        }


        while (curr_num_col < req_num_col) {
            if(curr_num_col % key_size == 0){
                std::vector<uint8_t> last_col;

                //emplace last column in last_col
                int fin_pos = curr_num_col - 1;
                last_col.push_back(expanded_key[0][fin_pos]);
                last_col.push_back(expanded_key[1][fin_pos]);
                last_col.push_back(expanded_key[2][fin_pos]);
                last_col.push_back(expanded_key[3][fin_pos]);

                //rotate, substitute, apply round constant
                rot_word(last_col);
                sub_word(last_col);
                op_rcon(last_col);

                int op_pos = curr_num_col - key_size;//inc forward depending on key size

                for(int i = 0; i < key_size; i++){
                    last_col[0] = last_col[0] ^ expanded_key[0][op_pos];
                    last_col[1] = last_col[1] ^ expanded_key[1][op_pos];
                    last_col[2] = last_col[2] ^ expanded_key[2][op_pos];
                    last_col[3] = last_col[3] ^ expanded_key[3][op_pos];

                    op_pos++;

                    //push back operated value
                    expanded_key[0].push_back(last_col[0]);
                    expanded_key[1].push_back(last_col[1]);
                    expanded_key[2].push_back(last_col[2]);
                    expanded_key[3].push_back(last_col[3]);

                    curr_num_col++;
                }
            }
        }


    }else{//256 differs apply subword every multiple of 4

    }
}

void KeyScheduler::rot_word(std::vector<uint8_t> &word){
    uint8_t temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void KeyScheduler::sub_word(std::vector<uint8_t> &word){
    for(int i = 0; i < word.size(); i++){
        std::pair<int, int> row_col_pair = util::separate(word[i]);
        word[i] = util::s_box[row_col_pair.first][row_col_pair.second];
    }
}

void KeyScheduler::op_rcon(std::vector<uint8_t> &word){
    word[0] *= this -> rcon_val;
    this -> rcon_val *= 2;
}

std::vector<std::vector<uint8_t>> KeyScheduler::get_expanded_key(){
    return this -> expanded_key;
}
