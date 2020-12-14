#include "cmd_motion.h"

using namespace std;

Posn CmdMoveBase::move_h(TextModel *model) {
  Posn start = model->get_posn();
  if (start.x > 0)
    return Posn(start.x - 1, start.y);
  else
    return start;
}

// Right
Posn CmdMoveBase::move_l(TextModel *model) {
  Posn start = model->get_posn();
  if (start.x + 1 < model->get_line_at(start.y).size())
    return Posn(start.x + 1, start.y);
  else
    return start;
}

Posn CmdMoveBase::move_k(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  int new_x = x;
  int new_y = y;
  if (y > 0) {
    new_y = y - 1;
    int next_x = model->get_lines()->at(y - 1).size();
    if (x >= next_x) {
      new_x = max(0, next_x - 1);
    }
  }
  return Posn(new_x, new_y);
}

Posn CmdMoveBase::move_j(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  int new_x = x;
  int new_y = y;
  if (y < model->get_lines()->size() - 1) {
    new_y = y + 1;
    int next_x = model->get_lines()->at(y + 1).size();
    if (x >= next_x) {
      new_x = max(0, next_x - 1);
    }
  }
  return Posn(new_x, new_y);
}

Posn CmdMoveBase::move_0(TextModel *model) { return Posn(0, model->get_y()); }

Posn CmdMoveBase::move_caret(TextModel *model) {
  string ln = model->get_line_at();
  size_t ln_size = ln.size();
  ln_size = ln_size == 0 ? 0 : ln_size - 1;
  size_t first = ln.find_first_not_of(' ');
  // If no non-whitespace characters found move to end of line
  return Posn(first == string::npos ? ln_size : first, model->get_y());
}

Posn CmdMoveBase::move_dollar(TextModel *model) {
  string ln = model->get_line_at();
  size_t ln_size = ln.size();
  ln_size = ln_size == 0 ? 0 : ln_size - 1;
  return Posn(ln_size, model->get_y());
}

// Returns posn of start of next word
Posn CmdMoveBase::start_next_word(TextModel *model) {
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
Posn CmdMoveBase::start_prev_word(TextModel *model) {
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

CmdMove::CmdMove(char c) : move_type{c} {}

void CmdMove::exec(TextModel *model) {
  switch (move_type) {
    case 'h': {
      Posn end = move_h(model);
      if (model->get_posn() == end)
        beep();
      else
        model->set_posn(end);
      break;
    }
    case 'l': {
      Posn end = move_l(model);
      if (model->get_posn() == end)
        beep();
      else
        model->set_posn(end);
      break;
    }
    case 'k': {
      Posn end = move_k(model);
      if (model->get_posn() == end)
        beep();
      else
        model->set_posn(end);
      break;
    }
    case 'j': {
      Posn end = move_j(model);
      if (model->get_posn() == end)
        beep();
      else
        model->set_posn(end);
      break;
    }
    case '0': {
      Posn end = move_0(model);
      model->set_posn(end);
      break;
    }
    case '^': {
      Posn end = move_caret(model);
      model->set_posn(end);
      break;
    }
    case '$': {
      Posn end = move_dollar(model);
      model->set_posn(end);
      break;
    }
    case 'b': {
      Posn end = start_prev_word(model);
      model->set_posn(end);
      break;
    }
    case 'w': {
      Posn end = start_next_word(model);
      model->set_posn(end);
      break;
    }
    default:
      throw invalid_argument("Unrecognized move_type: " + to_string(move_type));
      break;
  }
}

CmdcC::CmdcC(char c) : change_type{c} {}

void CmdcC::clear_core(TextModel *model, Posn end) {
  Posn cur = model->get_posn();
  if (cur != end) {
    if (cur.y == end.y) {
      string ln = model->get_line_at();
      if (end.x > cur.x) {
        model->set_line_at(ln.erase(cur.x, end.x - cur.x));
      } else {
        model->set_line_at(ln.erase(end.x, cur.x - end.x));
        model->set_x(end.x);
      }
    } else {
      if (cur.y > end.y) {
        for (int i = 0; i < abs(cur.y - end.y); i++) {
          model->set_y(model->get_y() - 1);
          model->delete_line(model->get_y());
        }
        // Clear last line
        model->clear_line(model->get_y());
      } else {
        // Delete lines through end.y
        for (int i = 0; i < abs(cur.y - end.y); i++) {
          model->delete_line(model->get_y());
        }
        model->clear_line(model->get_y());
        // If you delete the last line, need to move y back
        if (model->get_y() >= model->get_lines()->size()) {
          model->set_y(model->get_lines()->size() - 1);
        }
      }
      model->set_x(0);
    }
  }
}

void CmdcC::exec(TextModel *model) {
  switch (change_type) {
    case 'h': {
      Posn end = move_h(model);
      clear_core(model, end);
      break;
    }
    case 'l': {
      Posn end = move_l(model);
      clear_core(model, end);
      break;
    }
    case 'k': {
      Posn end = move_k(model);
      clear_core(model, end);
      break;
    }
    case 'j': {
      Posn end = move_j(model);
      clear_core(model, end);
      break;
    }
    case '0': {
      Posn end = move_0(model);
      clear_core(model, end);
      break;
    }
    case '^': {
      Posn end = move_caret(model);
      clear_core(model, end);
      break;
    }
    case '$': {
      Posn end = move_dollar(model);
      clear_core(model, end);
      break;
    }
    case 'c': {
      model->clear_line(model->get_y());
      model->set_x(0);
      break;
    }
    case 'b': {
      Posn end = start_prev_word(model);
      clear_core(model, end);
      break;
    }
    case 'w': {
      Posn end = start_next_word(model);
      clear_core(model, end);
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

void CmddD::delete_core(TextModel *model, Posn end) {
  Posn cur = model->get_posn();
  if (cur != end) {
    if (cur.y == end.y) {
      string ln = model->get_line_at();
      if (end.x > cur.x) {
        model->set_line_at(ln.erase(cur.x, end.x - cur.x));
      } else {
        model->set_line_at(ln.erase(end.x, cur.x - end.x));
        model->set_x(end.x);
      }
    } else {
      // Delete cur lin
      model->delete_line(model->get_y());
      if (cur.y > end.y) {
        for (int i = 0; i < abs(cur.y - end.y); i++) {
          model->set_y(model->get_y() - 1);
          model->delete_line(model->get_y());
        }
      } else {
        // Delete lines through end.y
        for (int i = 0; i < abs(cur.y - end.y); i++) {
          model->delete_line(model->get_y());
        }
        // If you delete the last line, need to move y back
        if (model->get_y() >= model->get_lines()->size()) {
          model->set_y(model->get_lines()->size() - 1);
        }
      }
      model->set_x(0);
    }
  }
}

void CmddD::exec(TextModel *model) {
  switch (del_type) {
    case 'h': {
      Posn end = move_h(model);
      delete_core(model, end);
      break;
    }
    case 'l': {
      Posn end = move_l(model);
      delete_core(model, end);
      break;
    }
    case 'k': {
      Posn end = move_k(model);
      delete_core(model, end);
      break;
    }
    case 'j': {
      Posn end = move_j(model);
      delete_core(model, end);
      break;
    }
    case '0': {
      Posn end = move_0(model);
      delete_core(model, end);
      break;
    }
    case '^': {
      Posn end = move_caret(model);
      delete_core(model, end);
      break;
    }
    case '$': {
      Posn end = move_dollar(model);
      delete_core(model, end);
      break;
    }
    case 'd': {
      // Delete line and move cusor to next line
      int y = model->get_y();
      // Don't clear if only 1 line remaining
      if (model->get_lines()->size() == 1)
        model->clear_line(y);
      else
        model->delete_line(y);
      // If deleted last line, need to move y down
      if (y == model->get_lines()->size() && y != 0) y -= 1;
      model->set_x(0);
      model->set_y(y);
      break;
    }
    case 'w': {
      Posn end = start_next_word(model);
      delete_core(model, end);
      break;
    }
    case 'b': {
      Posn end = start_prev_word(model);
      delete_core(model, end);
      break;
    }
    case 'x': {
      model->delete_char(model->get_x() + 1, model->get_y());
      // If cursor deleted last char in line, shift cursor back
      if (model->get_x() == model->get_line_at().size() && model->get_x() > 0)
        model->set_x(model->get_x() - 1);
      break;
    }
    case 'X': {
      if (model->get_x() > 0) {
        model->delete_char(model->get_x(), model->get_y());
        model->set_x(model->get_x() - 1);
      }
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
