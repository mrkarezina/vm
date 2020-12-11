#include "controller.h"

using namespace std;

void Controller::initilize_write_cmd_map() {}

void Controller::initilize_insert_cmd_map() {}

Controller::Controller(TextModel *model) : model{model} {}

unique_ptr<CmdBase> Controller::parse_input() {
  set_escdelay(0);
  int c = getch();
  unique_ptr<CmdBase> cmd = nullptr;

  string cur_cmd = model->get_cmd_so_far();
  cur_cmd += c;

  // TODO: Colon commands to support
  // :w :q :wq :q! :r :0 :$ :line-number
  if (cur_cmd == ":wq") cmd = make_unique<CmdSaveExit>();

  // TODO: / and ? commands
  // Search forward and backward

  // Command mode
  if (!model->is_write_mode()) {
    if (cur_cmd == "i" || cur_cmd == "a" || cur_cmd == "I" || cur_cmd == "A") {
      cmd = make_unique<CmdInsert>(c);
    }

    // Handle single character movement commands
    if (cur_cmd == "h" || cur_cmd == "j" || cur_cmd == "k" || cur_cmd == "l")
      cmd = make_unique<CmdMove>(c);
  }

  if (c == KEY_UP) cmd = make_unique<CmdMove>('k');
  if (c == KEY_DOWN) cmd = make_unique<CmdMove>('j');

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
    model->set_cmd_so_far(cur_cmd);
  } else {
    // Reset if command was matched
    model->set_cmd_so_far("");
  }

  return cmd;
}
