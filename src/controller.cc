#include "controller.h"

using namespace std;

Controller::Controller(TextModel *model) : model{model} {
  parser = make_unique<Parser>(model);
}

unique_ptr<CmdBase> Controller::parse_input() {
  set_escdelay(0);
  int c = getch();
  unique_ptr<CmdBase> cmd = nullptr;

  string cur_cmd = model->get_cmd_so_far();
  cur_cmd += c;
  model->set_cmd_so_far(cur_cmd);

  // TODO: parsing hiearchy matters, how to make code less brittle?

  // Check if numeric prefix -> cur_cmd[0] == is num
  // If numeric prefix an letter, substr numeric prefix and create int
  // Parse remaining command
  // If at end num_prefix != 0, then call gen multi command with command and
  // num_prefix

  // First try to parse a multiplier before any commands
  // Will stall command until multiplier parsing finished
  cmd = parser->parse_multiplier(c);

  if (cmd == nullptr && !model->is_write_mode()) {
    cmd = parser->parse_command_mode(c);
  }

  if (cmd == nullptr) {
    cmd = parser->parse_any_mode(c);
  }

  if (cmd == nullptr) {
    // Stall if command incomplete
    cmd = make_unique<CmdStall>();
  } else {
    // Reset if command was matched
    model->set_cmd_so_far("");
    parser->reset();
  }

  return cmd;
}
