#include "controller.h"

using namespace std;

void Controller::initilize_write_cmd_map() {}

void Controller::initilize_insert_cmd_map() {}

Controller::Controller(TextModel *model) : model{model} {}

unique_ptr<CmdBase> Controller::parse_input() {
  set_escdelay(0);
  int c = getch();

  if (c == 10) return make_unique<CmdEnter>();
  if (c == 127 || c == KEY_BACKSPACE) return make_unique<CmdDel>();
  if (c == 27 && model->is_write_mode()) return make_unique<CmdEsc>();
  if (c == 'i' && !model->is_write_mode()) {
    model->toggle_mode();
    return make_unique<CmdStall>();
  };

  if (!model->is_write_mode()) {
    // Handle single character command mode commands
    if (c == 'h' || c == 'j' || c == 'k' || c == 'l')
      return make_unique<CmdMove>(c);
  } else {
    return make_unique<CmdWrite>(c);
  }

  // If nothing matched return stall command and keep parsing
  return make_unique<CmdStall>();
}
