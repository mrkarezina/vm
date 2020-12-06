#include "controller.h"

using namespace std;

void Controller::initilize_write_cmd_map() {
    
}

void Controller::initilize_insert_cmd_map() {
    
}

Controller::Controller() {

}

unique_ptr<CmdBase> Controller::parse_input() {
    set_escdelay(0);
    int c = getch();
    
    if(c == 10) return make_unique<CmdWrite>('\n');
    if(c == 127 || c==KEY_BACKSPACE) return make_unique<CmdDel>();
    if(c == 27) return make_unique<CmdEsc>();

    // In insert mode write pretty much any character to screen, except esc
    return make_unique<CmdWrite>(c);
}

