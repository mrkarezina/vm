#include "cmd-basic.h"

using namespace std;

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (model->is_write_mode()) model->addChar(toWrite, x, y);
  model->setX(x + 1);
}

void CmdEnter::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (model->is_write_mode()) {
    model->new_line(x, y);
    model->setX(0);
    model->setY(y + 1);
  } else {
    if (y < model->getLines()->size() - 1) {
      model->setX(0);
      model->setY(y + 1);
    } else
      beep();
  }
}

void CmdBackspace::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (x > 0 || y > 0) {
    if (model->is_write_mode()) {
      if (x == 0) {
        // Set x to length of prev line
        int new_x = model->getLines()->at(y - 1).size();
        model->delete_line(x, y, true);
        model->setX(new_x);
        model->setY(y - 1);
      } else {
        model->delete_char(x, y);
        model->setX(x - 1);
      }
    } else {
      if (x == 0) {
        // Move to end and up
        int new_x = model->getLines()->at(y - 1).size();
        model->setX(new_x == 0 ? new_x : new_x - 1);
        model->setY(y - 1);
      } else {
        model->setX(x - 1);
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
      model->setX(model->getX() + 1);
      break;
    case 'A':
      model->setX(model->getLines()->at(model->getY()).size());
      break;
    case 'i':
      break;
    case 'I':
      model->setX(0);
      break;
    case 'o':
      model->new_line(0, model->getY() + 1);
      model->setY(model->getY() + 1);
      break;
    case 'O':
      model->new_line(0, model->getY());
      model->setY(model->getY());
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
  int x = model->getX();
  int y = model->getY();

  if (move == 'h') {
    if (x > 0)
      model->setX(x - 1);
    else
      beep();
  }

  if (move == 'l') {
    if (x + 1 < model->getLines()->at(y).size())
      model->setX(x + 1);
    else
      beep();
  }
  // Up
  if (move == 'k') {
    if (y > 0) {
      model->setY(y - 1);
      int new_x = model->getLines()->at(y - 1).size();
      if (x >= new_x) model->setX(new_x == 0 ? new_x : new_x - 1);
    } else
      beep();
  }
  // Down
  if (move == 'j') {
    if (y < model->getLines()->size() - 1) {
      model->setY(y + 1);
      int new_x = model->getLines()->at(y + 1).size();
      if (x >= new_x) model->setX(new_x == 0 ? new_x : new_x - 1);
    } else
      beep();
  }
}

void CmdStall::exec(TextModel *model) {}

void CmdSaveExit::exec(TextModel *model) {
  model->save_lines();
  model->set_render_loop_off();
  // That's all folks
}

Cmdf::Cmdf(char c) : to_find{c} {}

void Cmdf::exec(TextModel *model) {
  // Store previous occurance location in text model state
  // Restore prev loc or find first occurance
  // Move cursor to next occurance
  // If no next occurance don't move and beep

  int start_loc = model->getX();

  // Restore prev found loc and target char
  if (to_find == '\0') {
    to_find = model->state_str["cmd_f_prev_char"][0];
    // Move one past previous loc found, to find next
    start_loc = model->state_int["cmd_f_prev_loc"] + 1;
  }

  string ln = model->getLines()->at(model->getY());
  // Find first occruance past cur x
  size_t pos = ln.find(to_find, start_loc);
  // If not found stay at cur pos
  if (string::npos == pos) {
    pos = model->getX();
    beep();
  }
  model->setX(pos);
  // Store state for location where found and target char
  model->state_str["cmd_f_prev_char"] = to_find;
  model->state_int["cmd_f_prev_loc"] = pos;
}

CmdsS::CmdsS(char c) : sub_type{c} {}

void CmdsS::exec(TextModel *model) {
  switch (sub_type) {
    case 's':
      model->delete_char(model->getX() + 1, model->getY());
      break;
    case 'S':
      model->clear_line(model->getY());
      model->setX(0);
      break;
    default:
      throw invalid_argument("Unrecognized sub_type: " + to_string(sub_type));
      break;
  }
  model->toggle_mode();
}

CmdJump::CmdJump(char c) : jump_type{c} {}

// Returns posn of start of next word
posn start_next_word(TextModel *model) {
  // Begining of next word
  // If non left on line, start begining of next line
  string ln = model->get_line_at();
  size_t first = ln.find(' ', model->getX());
  if (first == string::npos) {
    if (model->getY() < model->getLines()->size() - 1) {
      return posn{0, model->getY() + 1};
    } else {
      return posn{0, model->getY()};
    }
  }
  return posn{(int)first + 1, model->getY()};
}

// Returns posn of start of prev word
posn start_prev_word(TextModel *model) {
  string ln = model->get_line_at();
  size_t first = ln.rfind(' ', model->getX() - 2);
  if (first == string::npos) {
    if (0 < model->getY()) {
      // New posn is end of previous line
      return posn{(int)model->get_line_at(model->getY() - 1).size(),
                  model->getY() - 1};
    } else {
      return posn{0, model->getY()};
    }
  }
  return posn{(int)first + 1, model->getY()};
}

void CmdJump::exec(TextModel *model) {
  string ln = model->getLines()->at(model->getY());
  size_t ln_size = ln.size();

  switch (jump_type) {
    case '0':
      model->setX(0);
      break;
    case '^': {
      // If no non-whitespace characters found move to end of line
      size_t first = ln.find_first_not_of(' ');
      ln_size = ln_size == 0 ? 0 : ln_size - 1;
      model->setX(first == string::npos ? ln_size : first);
      break;
    }
    case '$': {
      ln_size = ln_size == 0 ? 0 : ln_size - 1;
      model->setX(ln_size);
      break;
    }
    case 'b': {
      posn new_posn = start_prev_word(model);
      model->setX(new_posn.x);
      model->setY(new_posn.y);
      break;
    }
    case 'w': {
      posn new_posn = start_next_word(model);
      model->setX(new_posn.x);
      model->setY(new_posn.y);
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
      model->clear_line(model->getY());
      model->setX(0);
      break;
    }
    case '$': {
      // Clear past x posn
      model->set_line_at(ln.substr(0, model->getX()));
      break;
    }
    case 'w': {
      size_t first = ln.find_first_of(' ', model->getX());
      if (first == string::npos) {
        // Clear from cur posn to end of line
        model->set_line_at(ln.substr(0, model->getX()));
      } else {
        // Replace from cur posn to end of word
        model->set_line_at(
            ln.replace(model->getX(), first - model->getX(), ""));
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
      model->set_line_at(ln.substr(0, model->getX()));
      if (model->getX() > 0) model->setX(model->getX() - 1);
      break;
    }
    case 'd': {
      // Delete line and move cusor to next line
      model->delete_line(0, model->getY(), false);
      model->setX(0);
      break;
    }
    case 'w': {
      string ln = model->get_line_at();
      size_t first = ln.find_first_of(' ', model->getX());
      if (first == string::npos) {
        // Clear from cur posn to end of line
        model->set_line_at(ln.substr(0, model->getX()));
      } else {
        // Replace from cur posn to end of word
        model->set_line_at(
            ln.replace(model->getX(), first - model->getX(), ""));
      }
    }
    case 'x': {
      model->delete_char(model->getX() + 1, model->getY());
      // If cursor deleted last char in line, shift cursor back
      if (model->getX() == model->get_line_at().size() && model->getX() > 0)
        model->setX(model->getX() - 1);
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
      model->new_line(model->get_line_at().size(), model->getY());
      model->setY(model->getY() + 1);
      try {
        model->set_line_at(model->state_str["copied_data"]);
      } catch (std::out_of_range const e) {
      }
      break;
    }
    case 'P': {
      // Paste above cur line
      model->new_line(0, model->getY());
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
  model->setX(0);
}

posn CmdSearchBase::find_next_occurance(TextModel *model, std::string query,
                                        posn start_pos) {
  int cur_y = start_pos.y;
  int cur_x = start_pos.x;
  string ln = model->get_line_at(start_pos.y);
  size_t pos = ln.find(query, cur_x);
  cur_x = 0;
  while (pos == string::npos) {
    cur_y += 1;
    if (cur_y == model->getLines()->size()) cur_y = 0;
    ln = model->get_line_at(cur_y);
    pos = ln.find(query, cur_x);
    // Not found if entire text has been searched (looped back to start)
    if (cur_y == start_pos.y && pos == string::npos) return posn{-1, -1};
  }
  return posn{(int)pos, cur_y};
}

posn CmdSearchBase::find_prev_occurance(TextModel *model, std::string query,
                                        posn start_pos) {
  int cur_y = start_pos.y;
  int cur_x = start_pos.x;
  string ln = model->get_line_at(cur_y);
  size_t pos = ln.rfind(query, cur_x);
  // Correct rfind matching when start pos 0
  // Shouldn't because there are no prev characters
  if (cur_x == 0) pos = string::npos;
  while (pos == string::npos) {
    cur_y -= 1;
    if (cur_y < 0) cur_y = model->getLines()->size() - 1;
    ln = model->get_line_at(cur_y);
    cur_x = ln.size();
    pos = ln.rfind(query, cur_x);
    // Not found if entire text has been searched
    if (cur_y == start_pos.y && pos == string::npos) return posn{-1, -1};
  }
  return posn{(int)pos, cur_y};
}

CmdSearchStart::CmdSearchStart(char search_type, std::string query)
    : search_type{search_type}, query{query} {}

void CmdSearchStart::exec(TextModel *model) {
  // Save state between command for:
  // - search direction
  // - query
  // - lastest hit location x and y
  model->state_int["cmd_search_is_forward_search_direction"] =
      (search_type == '/');
  model->state_str["cmd_search_query"] = query;

  // Move cursor to begining of occurance of
  posn new_pos;
  if (search_type == '/')
    new_pos =
        find_next_occurance(model, query, posn{model->getX(), model->getY()});
  else
    new_pos =
        find_prev_occurance(model, query, posn{model->getX(), model->getY()});
  if (new_pos.x != -1) {
    model->setX(new_pos.x);
    model->setY(new_pos.y);
  } else {
    beep();
  }

  model->state_int["cmd_search_prev_x"] = model->getX();
  model->state_int["cmd_search_prev_y"] = model->getY();
}

CmdSearchNav::CmdSearchNav(char c) : nav_type{c} {}

void CmdSearchNav::exec(TextModel *model) {
  try {
    bool is_forward_search =
        model->state_int["cmd_search_is_forward_search_direction"];
    string query = model->state_str["cmd_search_query"];
    int x = model->state_int["cmd_search_prev_x"];
    int y = model->state_int["cmd_search_prev_y"];

    posn new_pos;
    if ((is_forward_search && nav_type == 'n') ||
        (!is_forward_search && nav_type == 'N')) {
      new_pos = find_next_occurance(model, query, posn{x + 1, y});
    }
    if ((!is_forward_search && nav_type == 'n') ||
        (is_forward_search && nav_type == 'N')) {
      new_pos = find_prev_occurance(model, query, posn{x != 0 ? x - 1 : 0, y});
    }
    model->state_int["cmd_search_prev_x"] = new_pos.x;
    model->state_int["cmd_search_prev_y"] = new_pos.y;
    model->setX(new_pos.x);
    model->setY(new_pos.y);
  } catch (std::out_of_range const e) {
    // If no search query provided
    beep();
  }
}
