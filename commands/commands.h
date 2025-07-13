#pragma once
#include <string>

namespace commands{
    void parseCommand(std::string &command);
    void parseCommand(std::string &command, std::string &value);

    void displayHelpMenu();
}
