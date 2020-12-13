#include "cmd_motion.h"

using namespace std;

// Returns posn of start of next word
Posn CmdMove::start_next_word(TextModel *model) {
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
Posn CmdMove::start_prev_word(TextModel *model) {
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
  int x = model->get_x();
  int y = model->get_y();

  string ln = model->get_lines()->at(model->get_y());
  size_t ln_size = ln.size();

  switch (move_type) {
    case 'h': {
      if (x > 0)
        model->set_x(x - 1);
      else
        beep();
      break;
    }
    case 'l': {
      if (x + 1 < model->get_lines()->at(y).size())
        model->set_x(x + 1);
      else
        beep();
      break;
    }
    case 'k': {
      if (y > 0) {
        model->set_y(y - 1);
        int new_x = model->get_lines()->at(y - 1).size();
        if (x >= new_x) model->set_x(new_x == 0 ? new_x : new_x - 1);
      } else
        beep();
      break;
    }
    case 'j': {
      if (y < model->get_lines()->size() - 1) {
        model->set_y(y + 1);
        int new_x = model->get_lines()->at(y + 1).size();
        if (x >= new_x) model->set_x(new_x == 0 ? new_x : new_x - 1);
      } else
        beep();
      break;
    }
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
      throw invalid_argument("Unrecognized move_type: " + to_string(move_type));
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
