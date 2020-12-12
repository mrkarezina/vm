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

  // TODO: / and ? commands
  // Search forward and backward

  // Command mode
  if (!model->is_write_mode()) {
    if (cur_cmd == "i" || cur_cmd == "a" || cur_cmd == "I" || cur_cmd == "A" ||
        cur_cmd == "o" || cur_cmd == "O") {
      cmd = make_unique<CmdInsert>(c);
    }
    if (cur_cmd == "w" || cur_cmd == "b" || cur_cmd == "0" || cur_cmd == "^" ||
        cur_cmd == "$") {
      cmd = make_unique<CmdJump>(c);
    }

    // Handle single character movement commands
    if (cur_cmd == "h" || cur_cmd == "j" || cur_cmd == "k" || cur_cmd == "l")
      cmd = make_unique<CmdMove>(c);

    // Search
    if (cur_cmd.size() > 1 && cur_cmd[0] == 'f') {
      cmd = make_unique<Cmdf>(c);
    }
    if (cur_cmd == ";") {
      cmd = make_unique<Cmdf>('\0');
    }

    // Substitute commands
    if (cur_cmd == "s" || cur_cmd == "S") cmd = make_unique<CmdsS>(c);

    // Clear commands
    if (cur_cmd == "cc" || cur_cmd == "cw" || cur_cmd == "c$")
      cmd = make_unique<CmdcC>(c);

    // Delete commands
    if (cur_cmd == "d$" || cur_cmd == "dd" || cur_cmd == "dw" || cur_cmd == "x")
      cmd = make_unique<CmddD>(c);

    // Copy commands
    if (cur_cmd == "yy") cmd = make_unique<CmdyY>(c);

    // Paste commands
    if (cur_cmd == "p" || cur_cmd == "P") cmd = make_unique<CmdPaste>(c);

    // Search commands
    // cur_cmd.substr(1, cur_cmd.size() - 2, get's _the_text_ from
    // /_the_text_[enter acsii]
    if ((cur_cmd[0] == '/' || cur_cmd[0] == '?') && cur_cmd.size() > 2 &&
        c == 10)
      cmd = make_unique<CmdSearchStart>(cur_cmd[0],
                                        cur_cmd.substr(1, cur_cmd.size() - 2));

    if (cur_cmd == "n" || cur_cmd == "N") cmd = make_unique<CmdSearchNav>(c);

    if (cur_cmd.substr(0, 2) == ":q" && c == 10) cmd = make_unique<CmdQuit>();
    if (cur_cmd.substr(0, 2) == ":w" && c == 10)
      cmd = make_unique<CmdSaveLines>();
    if (cur_cmd.substr(0, 3) == ":q!" && c == 10) cmd = make_unique<CmdQuit>();
    // TODO: Colon commands to support
    // :w :q :wq :q! :r :0 :$ :line-number
    if (cur_cmd.substr(0, 3) == ":wq" && c == 10)
      cmd = make_unique<CmdSaveExit>();

    // r command
  }

  if (c == KEY_UP) cmd = make_unique<CmdMove>('k');
  if (c == KEY_DOWN) cmd = make_unique<CmdMove>('j');

  // Command / write mode sensitive commands
  if (c == 10 && cmd == nullptr) cmd = make_unique<CmdEnter>();
  if (c == 127 || c == KEY_BACKSPACE) cmd = make_unique<CmdBackspace>();
  if (c == 27) cmd = make_unique<CmdEsc>();

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
