#include "cmd-basic.h"

#include "ncurses.h"

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  model->addChar(toWrite, x, y);

  // TODO: command takes window object to which to write to.
  int h;
  int w;
  getmaxyx(stdscr, h, w);
  // Max window size
  if(x >= w) {
    model->setX(0);
    model->setY(y + 1);
  } else {
    model->setX(x + 1);
  }
}
