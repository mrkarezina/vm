#include "history.h"

#include "text_model.h"

History::History() {
  change_since_write = 0;
  prev = nullptr;
}

void History::record_state(TextModel *model) {
  if (text_states.size() == 0 || *model->get_lines() != text_states.back()) {
    text_states.push_back(*model->get_lines());
    cursor_states.push_back(model->get_posn());
    change_since_write += 1;
  }
}

void History::record_prev_command(std::shared_ptr<CmdBase> cmd) {
  if (replace_prev_command) {
    set_prev_cmd(cmd);
  } else {
    replace_prev_command = true;
  }
}

void History::set_replace_prev_command(bool replace) {
  replace_prev_command = replace;
}

std::vector<std::string> History::pop_last_text_state() {
  auto state = text_states.back();
  text_states.pop_back();
  change_since_write -= 1;
  return state;
}

Posn History::pop_last_cursor_state() {
  auto state = cursor_states.back();
  cursor_states.pop_back();
  return state;
}

std::shared_ptr<CmdBase> History::get_prev_cmd() { return prev; }

void History::set_prev_cmd(std::shared_ptr<CmdBase> cmd) { prev = cmd; }

int History::state_size() { return text_states.size(); }
