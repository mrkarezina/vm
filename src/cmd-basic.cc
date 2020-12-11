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

void CmdDel::exec(TextModel *model) {
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

void CmdEsc::exec(TextModel *model) { model->toggle_mode(); }

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

CmdJump::CmdJump(char c) : jump_type{c} {}

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
    // TODO
    case 'b':
    default:
      throw invalid_argument("Unrecognized jump_type: " + to_string(jump_type));
      break;
  }
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
      model->clear_line(model->getY());
      model->setX(0);
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
