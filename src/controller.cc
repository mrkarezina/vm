#include "controller.h"

using namespace std;

void Controller::initilize_write_cmd_map() {}

void Controller::initilize_insert_cmd_map() {}

Controller::Controller(TextModel *model) : model{model} {}

unique_ptr<CmdBase> Controller::parse_input() {
  set_escdelay(0);
  int c = getch();
  unique_ptr<CmdBase> cmd = nullptr;

  string prev_cmd = model->get_cmd_so_far();
  prev_cmd += c;

  if (prev_cmd == ":wq") cmd = make_unique<CmdSaveExit>();

  // Command mode
  if (!model->is_write_mode()) {
    if (c == 'i') {
      model->toggle_mode();
      cmd = make_unique<CmdStall>();
    }
    // Handle single character movement commands
    if (c == 'h' || c == 'j' || c == 'k' || c == 'l')
      cmd = make_unique<CmdMove>(c);
  }

  // Command / write mode sensitive commands
  if (c == 10) cmd = make_unique<CmdEnter>();
  if (c == 127 || c == KEY_BACKSPACE) cmd = make_unique<CmdDel>();
  if (c == 27 && model->is_write_mode()) cmd = make_unique<CmdEsc>();

  // Any character written in write mode
  if (model->is_write_mode() && cmd == nullptr) {
    cmd = make_unique<CmdWrite>(c);
  }

  if (cmd == nullptr) {
    // Stall if command incomplete
    cmd = make_unique<CmdStall>();
    model->set_cmd_so_far(prev_cmd);
  } else {
    // Reset if command was matched
    model->set_cmd_so_far("");
  }

  return cmd;
}
