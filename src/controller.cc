#include "controller.h"

using namespace std;

void Controller::initilize_write_cmd_map() {
    
}

void Controller::initilize_insert_cmd_map() {
    
}

Controller::Controller() {

}

unique_ptr<CmdBase> Controller::parse_input() {
    int c = getch();
    // TODO: how to map to input
    return make_unique<CmdWrite>(c);
}

