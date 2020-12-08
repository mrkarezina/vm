#include "view-text.h"

using namespace std;

ViewText::ViewText() {
  int height, width;
  getmaxyx(stdscr, height, width);
  win = make_unique<Window>(width, height - 1, 0, 0);
}

void ViewText::draw_tilda(int x, int y) {
  // TODO: color tilda
  win->write_char('~', x, y);
}

int ceil_div(int x, int y) { return (x + y - 1) / y; }

// Add y offset from wrapping for a line
int ViewText::get_y_offset(int ln, TextModel *model) {
  int off = ceil_div(model->getLines()->at(ln).size(), win->getWidth());
  // If line is empty, still count as 1 y-offset
  return (off == 0 ? 1 : off);
}

int ViewText::get_view_y(TextModel *model) {
  int y_offset = 0;
  for (int i = 0; i < model->getY(); i++) {
    y_offset += get_y_offset(i, model);
  }
  // Line wrap for current line
  y_offset += model->getX() / win->getWidth();
  return y_offset;
}

int ViewText::get_view_x(TextModel *model) {
  return model->getX() % win->getWidth();
}

void ViewText::draw(TextModel *model) {
  win->erase_w();
  const shared_ptr<vector<string>> lines = model->getLines();

  int curx = get_view_x(model);
  int cury = get_view_y(model);

  int y_offset = 0;
  for (size_t i = 0; i < lines->size(); i++) {
    // TODO: scroll
    // if the y_offset is within the range of
    // cursor translated y then print

    win->write_line(lines->at(i), 0, y_offset);
    y_offset += get_y_offset(i, model);
  }

  for (size_t i = y_offset; i < win->getHeight(); i++) {
    draw_tilda(0, i);
  }

  win->move_w(curx, cury);
  win->refresh_w();
}
