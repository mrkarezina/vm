#include "cmd-basic.h"

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (model->get_write_mode()) model->addChar(toWrite, x, y);
  model->setX(x + 1);
}

void CmdEnter::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (model->get_write_mode()) model->new_line(x, y);
  model->setX(0);
  model->setY(y + 1);
}

void CmdDel::exec(TextModel *model) {
  int x = model->getX();
  int y = model->getY();
  if (x > 0 || y > 0) {
    if (x == 0) {
      // Set x to length of prev line
      int new_x = model->getLines()->at(y - 1).size();
      if (model->get_write_mode()) model->delete_line(x, y, true);
      model->setX(new_x);
      model->setY(y - 1);
    } else {
      if (model->get_write_mode()) model->delete_char(x, y);
      model->setX(x - 1);
    }
  }
}

void CmdEsc::exec(TextModel *model) { model->toggle_mode(); }

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
    // if(x==0) {
    //   std::cout << "SIZE: " << model->getLines()->at(y).size() << std::endl;
    // }
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
