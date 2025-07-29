#pragma once

#include <fstream>
#include <string>
#include <sys/types.h>

//destructor needed

class FileUtils{
    private:
    std::string file_name;
    std::string file_directory;
    std::ifstream raw_file;
    std::string file_content;
    bool is_valid;

    public:
    FileUtils(const std::string &file_directory);

    public:
    const std::string& getFileContent();
    void createCompressedFile(const std::string &bits);

    private:
    void readFileContent(const std::string &directory);
    void getFileNameFromPath(const std::string &directory, std::string &file_name);
    u_int8_t convertChunkToByte(const std::string &chunk);
};
