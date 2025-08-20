#pragma once
#include <string>
#include <unordered_set>

class Commands{

    private:
    std::unordered_set<std::string> compressables;
    std::unordered_set<std::string> decompressables;

    public:
    Commands(): compressables({{".txt", ".word"," "}}), decompressables({".txt", ".word"," "}){};
    void parseCommand(int command_count, char *command_array[]);

    private:
    void executeCommand(std::string &command);

    void executeCommand(std::string &command, std::string &value);

    void displayHelpMenu();
};
