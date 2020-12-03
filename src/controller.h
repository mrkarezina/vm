#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <string>
#include <memory>

#include "cmd-base.h"
#include "cmd-basic.h"
#include "ncurses.h"

class CmdBase;

class Controller {

    // Needs to figure out command
    // Provide input to command

    // Keyboard maps input to command -> initlizes command and stuff

    // Keep insert / write state in model
    // Each key try to map "cur_cmd" string to command
    // map for insert mode
    // map for cmd mode
    // store info to provide to cmd, ie: char


    private:
        std::map<std::string, std::string> write_cmd_map;
        std::map<std::string, std::string> insert_cmd_map;

        void initilize_write_cmd_map();
        void initilize_insert_cmd_map();
    public:
        Controller();
        std::unique_ptr<CmdBase> parse_input();

};

#endif
