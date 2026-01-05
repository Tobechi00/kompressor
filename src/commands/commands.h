#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <unordered_set>

class Commands{

    private:
    std::unordered_set<std::string> compressables;
    std::unordered_set<std::string> decompressables;

    public:
    Commands(): decompressables({".kmp"}){};
    void parseCommand(int command_count, char *command_array[]);

    private:
    void executeCommand(std::string &command);

    void executeCommand(std::string &command, std::string &value);

    void displayHelpMenu();
};

#endif
