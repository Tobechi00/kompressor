#pragma once

#include <fstream>
#include <string>

//destructor needed

class FileUtils{
    private:
    std::string file_name;
    std::string file_directory;
    std::ifstream raw_file;
    std::string file_content;
    bool is_read;

    public:
    FileUtils(const std::string &n_file_directory);

    public:
    const std::string& getFileContent();

    private:
    void readFileContent(const std::string &directory);
};
