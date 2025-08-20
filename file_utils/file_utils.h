#pragma once

#include <filesystem>
#include <fstream>
#include <string>

//destructor needed

class FileUtils{
    private:
    std::filesystem::path file_path;
    std::ifstream raw_file;
    std::string file_content;
    bool is_valid;

    public:
    FileUtils(const std::string &file_directory);

    public:
    const std::string& getFileContent();
    void createCompressedFile(const std::string &bits);
    void createFile(const std::string &content);

    private:
    void readFileContent(const std::string &directory);
    uint8_t convertChunkToByte(const std::string &chunk);
};
