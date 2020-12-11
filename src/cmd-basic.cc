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
  switch (jump_type) {
    case '0':
      model->setX(0);
      break;
    case '^': {
      // If no non-whitespace characters found move to end of line
      string ln = model->getLines()->at(model->getY());
      size_t first = ln.find_first_not_of(' ');
      size_t ln_size = ln.size();
      ln_size = ln_size == 0 ? 0 : ln_size - 1;
      model->setX(first == string::npos ? ln_size : first);
      break;
    }
    // TODO
    case 'b':
    case 'e': {
      break;
    }
    default:
      throw invalid_argument("Unrecognized jump_type: " + to_string(jump_type));
      break;
  }
}
