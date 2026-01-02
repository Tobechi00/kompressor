# Kompressor
Kompressor is a text file compression tool based on the huffman compression algorithm.

## how it works:
kompressor makes use of the huffman compression algorithm, which works by creating replacement codes for characters based on their frequencies in an attempt to reduce file sizes, it does this by assigning shorter generated codes to characters that appear more frequently and longer codes to characters which appear less often

### Compression:
characters are read from a text file and assigned prefix free codes based on their respective frequencies,
characters that appear more often get assigned shorter codes, while characters that appear less are assigned longer codes.
a dictionary is then constructed mapping each character to its respective code, this dictionary is placed in the output files header for decomprssion.
when codes are stored they are also padded with zeros if need be to even out their structure
seeing as how code lengths may deviate from the standard byte size (* multiples of 8)

the final compressed file is structured as so:

byte 0 to byte 4 = huffman_code_map_size;

byte 5 to (byte 5 + huffman_code_map_size) = huffman_code map;

byte (byte 5 + huffman_code_map_size(* +padding if (code_size % 8) != 0)) + 1 forwards = file content;

our codemap is constructed as so:

[character]|[huffman_code_len]|[huffman_code]

we store the additional code length to aid in the decoding process
it dictates where to stop when accumulating the huffman code.

### Decompression:
during decompression the first 4 bytes are read to get the length of the code map, this tells us where to stop in search of the map.
this implementation was specifically made with only utf-8(variable width) encoding in mind, and by implication also works for single width text formats like ascii. when decoding the code map the first 8bits / 1byte tells us how many bytes the character we are currently on contains.

eg: the bit representation of the letter A is 01000001
by reading this bit representation we notice the most significant bit is a zero, this tells us that this character only takes up a single byte
which helps us know where to end our character search and where to start our code length search.

utf-8 decoding pattern used:

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

in this implementation the code length portion of each entry of our map is 32 bits / 4 bytes, I selected a 32bit length to accomodate for a possible case of extremly high utf-8 character diversity
(i.e: a mix of english, chinese, japanese and multiple other characters);

the map is then used to convert the remaining bytes back to their character form.

# how to run
ensure you have cmake and a c++ environment installed

clone this repo:
```
git clone Tobechi00/kompressor
```

once done create a build folder in the main directory and cd into it:
```
mkdir build
cd build
```

then run:
```
cmake ..
```
once finished build the project from inside the build directory using:
```
cmake --build .
```

## Kompressor commands
for now kompressor has only three commands:

to compress run:
```
./kompressor -c "file path"
```
or
```
./kompressor --compress "file path"
```

to decompress a file with a .kmp extension run:
```
./kompressor -d "file path"
```
or
```
./kompressor --decompress "file path"
```

for a list of all commands run:
```
./kompressor -h
```
or
```
./kompressor --help
```

p.s I put a few test texts in the test_text folder incase you want to give it a try ;)


# known issues / flaws

## lack of robust file detection:
my proposed method of detecting file type was by checking the inputted files extension against a set of recognised text files
but on second thought I decided to avoid going down that route becaue there exists a large number of extentions which when used
don't always count as traditional text files, but do contain text and are, by definition text files. i.e: code files (.cpp, .cxx, .java, .py) which can be compressed using this project.

## limited file support:
this implementation only supports text files or files that only contain text, any other file type ie. images, could create unexpected results
