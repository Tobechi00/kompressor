#include "aes_encryption.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

const int aes_encryption::poly_rs_8[4] = {4, 3, 1, 0};

const uint8_t aes_encryption::s_box[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

const uint8_t aes_encryption::galois_field[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};

aes_encryption::aes_encryption(std::string file_name, std::string user_key){
    std::ifstream file_stream(file_name);


    //file is empty??
    if(!file_stream.is_open()){
        std::cout << "error occurred while opening file";
        return;
    }

    int length = user_key.length();

    int aes_version =
    (length == 16) ? 16 :
    (length == 24) ? 24 :
    (length == 32) ? 32 : -1;

    if(aes_version == -1){
        //err
        return;
    }
    //use PKCS#7 to pad block if not enough bytes
    // padding is determined based on the number of unfilled bytes (ie if 9 bytes 0x09)

    //16bit block in column major order
    char block[16];

    while(file_stream.read(block, 16)){
        int bytes_read = file_stream.gcount();

        char state[4][4];

        int block_ctr = 0;
        int last_col = 0;
        int last_row;

        //fill state & sub bytes phase
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){

                if(block_ctr == bytes_read){
                    last_col = col;
                    last_row = row;
                    break;
                }


                int upper_nibble = static_cast<int>((block[block_ctr] & 0xF0) >> 4);
                int lower_nibble = static_cast<int>(block[block_ctr] & 0x0F);

                //populate state with sbox mapped values
                state[row][col] = s_box[upper_nibble][lower_nibble];

                block_ctr++;
            }

            //if read up to number of bytes read
            if(block_ctr == bytes_read){
                break;
            }
        }

        uint8_t rem_bytes = static_cast<uint8_t>(16 - bytes_read);//0x0rembytes;

        //pad if not exactly 16 bytes (PKCS#7)
        if(rem_bytes > 0){
            for(int row = last_row; row < 4; row++){
                for(int col = last_col; col < 4; col++){
                    state[row][col] = rem_bytes;
                }
            }
        }

        //shift rows
        // row 1: 0;
        // row 2: 1;
        // row 3: 2;
        // row 4: 1;

        //shift r2 //todo: abstract to fun
        uint8_t fst = state[1][0];
        state[1][0] = state[1][1];
        state[1][1] = state[1][2];
        state[1][2] = state[1][3];
        state[1][3] = fst;

        //shift r3
        fst = state[2][0];
        uint8_t scnd = state[2][1];
        state[2][0] = state[2][2];
        state[2][1] = state[2][3];
        state[2][2] = fst;
        state[2][3] = scnd;

        //shift r4
        fst = state[3][0];
        scnd = state[3][1];
        uint8_t trd = state[3][2];
        state[3][0] = state[3][3];
        state[3][1] = fst;
        state[3][2] = scnd;
        state[3][3] = trd;


        //mix columns with galois field

        uint8_t mix_state[4][4];// holds state after mix_column op

        int col = 0;
        int row = 0;

	for(int st_col = 0; st_col < 4; st_col++){
            for(int gf_row = 0; gf_row < 4; gf_row++){
                uint8_t res = 0x00; //result

                for(int gf_col = 0; gf_col < 4; gf_col++){

                    int st_row = gf_col;

                    res = res ^ mix_col(galois_field[gf_row][gf_col], state[st_row][st_col]);
                }

                mix_state[row][col] = res;
                // std::cout << row << " -- " << col << "\n";

                if(row == 3){
                    row = 0;
                    col++;
                }else{
                    row++;
                }
            }
        }

	//ADD ROUND KEY POR FAVOR
	//xor state key with round key
	//first stage round key will be the initial key
	//remaining keys are generated by performing (rotword, subword and rconn(round constant)) then xoring with values from initial key
    }




}





uint8_t aes_encryption::mix_col(uint8_t field_val, uint8_t state_val){
    //g(2^3) == 8
        if(field_val == 0x01){//multiplying by 1 nets you the same value
            return state_val;
        }

        std::vector<int> field_non_z;
        std::vector<int> state_non_z;

        std::uint8_t mask = 0x80; //10000000

        //use mask to check if each bit is set in both values
        for(int i = 7; i >= 0; i--){

            uint8_t field_op = field_val | mask;
            uint8_t state_op = state_val | mask;

            if(field_val == field_op){
                field_non_z.push_back(i);
            }

            if(state_val == state_op){
                state_non_z.push_back(i);
            }


            mask = mask >> 1;//shift mask bit backwards
        }


        std::map<int, int> mul_val;//discard all keys with non one values/ cancel all values with duplicates out

        //irreducible polynomial theorem for values greater than 7 (==8) after addition, reduce to: x^4 + x^3 + x + 1
        //where these values  reflect the bit positions which are set

        //multiply all values of field with values of state
        for(int f_val : field_non_z){

            for(int s_val : state_non_z){
                int res = f_val + s_val;

                if(res == 8){//irreducable polynomial theorem
                    for(int i : poly_rs_8){
                        mul_val[i]++;
                    }
                }else{
                    mul_val[res]++;
                }
            }
        }
        //field_non_z * state_non_z;

        uint8_t fin_val = 0x00;

        for(const auto pair : mul_val){
            uint8_t mask = 0x01;//00000001

            if(pair.second % 2 != 0){//all with an even number of keys cross each other, odd keys have a reminant
                mask = mask << pair.first;
                fin_val = fin_val | mask;
            }
        }

        return fin_val;
}
