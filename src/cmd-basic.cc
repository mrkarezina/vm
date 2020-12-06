#include "cmd-basic.h"

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  // If enter
  if (toWrite == '\n') {
    model->new_line(x, y);
    model->setX(0);
    model->setY(y + 1);
  } else {
    model->addChar(toWrite, x, y);
    model->setX(x + 1);
  }
}

void CmdDel::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (x > 0 || y > 0) {
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
  }
}

void CmdEsc::exec(TextModel *model) {
  // Change text model state to insert / write model
  model->toggle_mode();
}
