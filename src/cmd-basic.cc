#include "cmd-basic.h"

using namespace std;

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (model->is_write_mode()) model->add_char(toWrite, x, y);
  model->set_x(x + 1);
}

void CmdEnter::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (model->is_write_mode()) {
    model->new_line(x, y);
    model->set_x(0);
    model->set_y(y + 1);
  } else {
    if (y < model->get_lines()->size() - 1) {
      model->set_x(0);
      model->set_y(y + 1);
    } else
      beep();
  }
}

void CmdBackspace::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (x > 0 || y > 0) {
    if (model->is_write_mode()) {
      if (x == 0) {
        // Set x to length of prev line
        int new_x = model->get_lines()->at(y - 1).size();
        model->delete_line(x, y, true);
        model->set_x(new_x);
        model->set_y(y - 1);
      } else {
        model->delete_char(x, y);
        model->set_x(x - 1);
      }
    } else {
      if (x == 0) {
        // Move to end and up
        int new_x = model->get_lines()->at(y - 1).size();
        model->set_x(new_x == 0 ? new_x : new_x - 1);
        model->set_y(y - 1);
      } else {
        model->set_x(x - 1);
      }
    }
  } else {
    beep();
  }
}

void CmdEsc::exec(TextModel *model) {
  if (model->is_write_mode())
    model->toggle_mode();
  else
    model->set_cmd_so_far("");
}

CmdInsert::CmdInsert(char c) : insert_type{c} {}

void CmdInsert::exec(TextModel *model) {
  switch (insert_type) {
    case 'a':
      model->set_x(model->get_x() + 1);
      break;
    case 'A':
      model->set_x(model->get_lines()->at(model->get_y()).size());
      break;
    case 'i':
      break;
    case 'I':
      model->set_x(0);
      break;
    case 'o':
      model->new_line(model->get_lines()->at(model->get_y()).size(),
                      model->get_y());
      model->set_y(model->get_y() + 1);
      model->set_x(0);
      break;
    case 'O':
      model->new_line(0, model->get_y());
      model->set_y(model->get_y());
      model->set_x(0);
      break;
    default:
      throw invalid_argument("Unrecognized insert_type: " +
                             to_string(insert_type));
      break;
  }
  model->toggle_mode();
}

CmdMove::CmdMove(char c) : move{c} {}

void CmdMove::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();

  if (move == 'h') {
    if (x > 0)
      model->set_x(x - 1);
    else
      beep();
  }

  if (move == 'l') {
    if (x + 1 < model->get_lines()->at(y).size())
      model->set_x(x + 1);
    else
      beep();
  }
  // Up
  if (move == 'k') {
    if (y > 0) {
      model->set_y(y - 1);
      int new_x = model->get_lines()->at(y - 1).size();
      if (x >= new_x) model->set_x(new_x == 0 ? new_x : new_x - 1);
    } else
      beep();
  }
  // Down
  if (move == 'j') {
    if (y < model->get_lines()->size() - 1) {
      model->set_y(y + 1);
      int new_x = model->get_lines()->at(y + 1).size();
      if (x >= new_x) model->set_x(new_x == 0 ? new_x : new_x - 1);
    } else
      beep();
  }
}

void CmdStall::exec(TextModel *model) {}

void CmdMultiCommand::add_command(std::unique_ptr<CmdBase> cmd) {
  commands.emplace_back(std::move(cmd));
}

void CmdMultiCommand::exec(TextModel *model) {
  for (auto &command : commands) {
    model->apply(std::move(command));
  }
}

void CmdSaveLines::exec(TextModel *model) { model->save_lines(); }

void CmdQuit::exec(TextModel *model) { model->set_render_loop_off(); }

CmdSaveExit::CmdSaveExit() {
  add_command(std::move(make_unique<CmdSaveLines>()));
  add_command(std::move(make_unique<CmdQuit>()));
}

CmdLineSelection::CmdLineSelection(string selection) : selection{selection} {}

void CmdLineSelection::exec(TextModel *model) {
  if (selection == "0") {
    model->set_y(0);
  } else if (selection == "$") {
    model->set_y((int)model->get_lines()->size() - 1);
  } else {
    int line = stoi(selection);
    model->set_y(min(line, (int)model->get_lines()->size() - 1));
  }
  model->set_x(0);
}

Cmdf::Cmdf(char c) : to_find{c} {}

void Cmdf::exec(TextModel *model) {
  // Store previous occurance location in text model state
  // Restore prev loc or find first occurance
  // Move cursor to next occurance
  // If no next occurance don't move and beep

  int start_loc = model->get_x();

  // Restore prev found loc and target char
  if (to_find == '\0') {
    to_find = model->state_str["cmd_f_prev_char"][0];
    // Move one past previous loc found, to find next
    start_loc = model->state_int["cmd_f_prev_loc"] + 1;
  }

  string ln = model->get_lines()->at(model->get_y());
  // Find first occruance past cur x
  size_t pos = ln.find(to_find, start_loc);
  // If not found stay at cur pos
  if (string::npos == pos) {
    pos = model->get_x();
    beep();
  }
  model->set_x(pos);
  // Store state for location where found and target char
  model->state_str["cmd_f_prev_char"] = to_find;
  model->state_int["cmd_f_prev_loc"] = pos;
}

CmdsS::CmdsS(char c) : sub_type{c} {}

void CmdsS::exec(TextModel *model) {
  switch (sub_type) {
    case 's':
      model->delete_char(model->get_x() + 1, model->get_y());
      break;
    case 'S':
      model->clear_line(model->get_y());
      model->set_x(0);
      break;
    default:
      throw invalid_argument("Unrecognized sub_type: " + to_string(sub_type));
      break;
  }
  model->toggle_mode();
}

CmdJump::CmdJump(char c) : jump_type{c} {}

// Returns posn of start of next word
Posn start_next_word(TextModel *model) {
  // Begining of next word
  // If non left on line, start begining of next line
  string ln = model->get_line_at();
  size_t first = ln.find(' ', model->get_x());
  if (first == string::npos) {
    if (model->get_y() < model->get_lines()->size() - 1) {
      return Posn{0, model->get_y() + 1};
    } else {
      return Posn{0, model->get_y()};
    }
  }
  return Posn{(int)first + 1, model->get_y()};
}

// Returns posn of start of prev word
Posn start_prev_word(TextModel *model) {
  string ln = model->get_line_at();
  size_t first = ln.rfind(' ', model->get_x() - 2);
  if (first == string::npos) {
    if (0 < model->get_y()) {
      // New posn is end of previous line
      return Posn{(int)model->get_line_at(model->get_y() - 1).size(),
                  model->get_y() - 1};
    } else {
      return Posn{0, model->get_y()};
    }
  }
  return Posn{(int)first + 1, model->get_y()};
}

void CmdJump::exec(TextModel *model) {
  string ln = model->get_lines()->at(model->get_y());
  size_t ln_size = ln.size();

  switch (jump_type) {
    case '0':
      model->set_x(0);
      break;
    case '^': {
      // If no non-whitespace characters found move to end of line
      size_t first = ln.find_first_not_of(' ');
      ln_size = ln_size == 0 ? 0 : ln_size - 1;
      model->set_x(first == string::npos ? ln_size : first);
      break;
    }
    case '$': {
      ln_size = ln_size == 0 ? 0 : ln_size - 1;
      model->set_x(ln_size);
      break;
    }
    case 'b': {
      Posn new_posn = start_prev_word(model);
      model->set_x(new_posn.x);
      model->set_y(new_posn.y);
      break;
    }
    case 'w': {
      Posn new_posn = start_next_word(model);
      model->set_x(new_posn.x);
      model->set_y(new_posn.y);
      break;
    }
    default:
      throw invalid_argument("Unrecognized jump_type: " + to_string(jump_type));
      break;
  }
}

CmdcC::CmdcC(char c) : change_type{c} {}

void CmdcC::exec(TextModel *model) {
  string ln = model->get_line_at();

  // cc, c$, cw
  switch (change_type) {
    case 'c': {
      model->clear_line(model->get_y());
      model->set_x(0);
      break;
    }
    case '$': {
      // Clear past x posn
      model->set_line_at(ln.substr(0, model->get_x()));
      break;
    }
    case 'w': {
      size_t first = ln.find_first_of(' ', model->get_x());
      if (first == string::npos) {
        // Clear from cur posn to end of line
        model->set_line_at(ln.substr(0, model->get_x()));
      } else {
        // Replace from cur posn to end of word
        model->set_line_at(
            ln.replace(model->get_x(), first - model->get_x(), ""));
      }
      break;
    }
    default:
      throw invalid_argument("Unrecognized change_type: " +
                             to_string(change_type));
      break;
  }
  model->toggle_mode();
}

CmddD::CmddD(char c) : del_type{c} {}

void CmddD::exec(TextModel *model) {
  switch (del_type) {
    case '$': {
      string ln = model->get_line_at();
      model->set_line_at(ln.substr(0, model->get_x()));
      if (model->get_x() > 0) model->set_x(model->get_x() - 1);
      break;
    }
    case 'd': {
      // Delete line and move cusor to next line
      model->delete_line(0, model->get_y(), false);
      model->set_x(0);
      break;
    }
    case 'w': {
      string ln = model->get_line_at();
      size_t first = ln.find_first_of(' ', model->get_x());
      if (first == string::npos) {
        // Clear from cur posn to end of line
        model->set_line_at(ln.substr(0, model->get_x()));
      } else {
        // Replace from cur posn to end of word
        model->set_line_at(
            ln.replace(model->get_x(), first - model->get_x(), ""));
      }
    }
    case 'x': {
      model->delete_char(model->get_x() + 1, model->get_y());
      // If cursor deleted last char in line, shift cursor back
      if (model->get_x() == model->get_line_at().size() && model->get_x() > 0)
        model->set_x(model->get_x() - 1);
      break;
    }
    default:
      throw invalid_argument("Unrecognized del_type: " + to_string(del_type));
      break;
  }
}

CmdyY::CmdyY(char c) : yank_type{c} {}

void CmdyY::exec(TextModel *model) {
  switch (yank_type) {
    case 'y': {
      // Save copied data to state
      model->state_str["copied_data"] = model->get_line_at();
      break;
    }
    default:
      throw invalid_argument("Unrecognized yank_type: " + to_string(yank_type));
      break;
  }
}

CmdPaste::CmdPaste(char c) : paste_type{c} {}

void CmdPaste::exec(TextModel *model) {
  switch (paste_type) {
    case 'p': {
      // Paste below cur line
      model->new_line(model->get_line_at().size(), model->get_y());
      model->set_y(model->get_y() + 1);
      try {
        model->set_line_at(model->state_str["copied_data"]);
      } catch (std::out_of_range const e) {
      }
      break;
    }
    case 'P': {
      // Paste above cur line
      model->new_line(0, model->get_y());
      try {
        model->set_line_at(model->state_str["copied_data"]);
      } catch (std::out_of_range const e) {
      }
      break;
    }
    default:
      throw invalid_argument("Unrecognized paste_type: " +
                             to_string(paste_type));
      break;
  }
  model->set_x(0);
}

