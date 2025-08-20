# Kompressor

Kompressor is a text file compression tool based on the huffman compression algorithm.

## how it works:

kompressor makes use of an the huffman compression algorithm:

### Compression:
characters are read from a text file and assigned prefix free codes based on their respective frequencies,
characters that appear more often get assigned shorter codes, while characters that appear less are assigned longer codes.
a dictionary is then constructed mapping each character to its respective code.

the final compressed file is structured as so:

byte 0 to byte 4 = huffman_code_map_size;

byte 5 to (byte 5 + huffman_code_map_size) = huffman_code map;

byte (byte 5 + huffman_code_map_size) + 1 forwards = file content;

our codemap is constructed as so: 

[char]|[huffman_code_len]|[huffman_code]

we store the additional code length to aid in the decoding process
it dictates where to stop when accumulating the huffman code.

### Decompression:
during decompression the first 4 bytes are read to get the length of the code map, this tells us where to stop in search of the map.
this implementation was specifically made with only utf-8(variable width) encoding in mind. when decoding the code map the first 8bits / 1byte tells us how many bytes the character we are currently on contains.

eg: the bit representation of the letter A is 01000001
by reading this bit representation we notice the most significant bit is a zero, this tells us that this character only takes up a single byte
which helps us know where to end our character search and where to start our code length search.

utf-8 decoding pattern used:

    if(first_byte[0] == '0'){//1 byte
        byte_len = 1;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '0'){//2 bytes
        byte_len = 2;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '1' && first_byte[3] == '0'){//3byte char
        byte_len = 3;
    }else if(first_byte[0] == '1' && first_byte[1] == '1' && first_byte[2] == '1' && first_byte[3] == '1' && first_byte[4] == '0'){//4byte char
        byte_len = 4;
    }

in this implementation the code length portion of each entry of our map is 8 bits / 1 byte which can hold from (0 - 255), through most of my testing, except in very rare theoretical cases, our code length always stayed below 255 in length.

after the length is extracted, its to extract our code from the ending point of our code, we move one bit forward and start the process all over for the other map entires, until our map length is equal to the length specified in the first 4 bytes.

the map is then used to convert the remaining bytes back to their character form.

# known issues / flaws

## lack of robust file detection:
my current method of detecting if files are text or not relies on extension checking, which isn't great as a file true type is usually somewhere in its magic number (first 8 bytes)

## limited file support:
this implementation only supports text files any other file type ie. images, could create unexpected results


