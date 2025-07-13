#include <iostream>
#include <string>
#include "commands/commands.h"


int main(int argc, char* argv[]){

    //arg[1] - command
    //arg[2] - filepath

    //parse args depending on number
    if(argc == 1){
        commands::displayHelpMenu();
    }else if(argc == 2){
        std::string first_command = argv[1];
        commands::parseCommand(first_command);
    }else if(argc == 3){
        std::string first_command = argv[1];
        std::string second_command = argv[2];
        commands::parseCommand(first_command, second_command);
    }else{
        std::cerr << "unrecognised command. See \'kompressor --help\'\n";
    }

}
