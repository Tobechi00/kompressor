#pragma once
#include <string>

namespace commands{
    void parseCommand(int command_count, char *command_array[]);

    void executeCommand(std::string &command);

    void executeCommand(std::string &command, std::string &value);

    void displayHelpMenu();
}
