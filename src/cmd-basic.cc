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
