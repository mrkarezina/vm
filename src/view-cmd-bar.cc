#include "view-cmd-bar.h"

using namespace std;

ViewCmdBar::ViewCmdBar() {
  int height, width;
  getmaxyx(stdscr, height, width);
  win = make_unique<Window>(width, 1, 0, height - 1);
}

void ViewCmdBar::draw(TextModel *model) {
  win->erase_w();
  if (model->is_write_mode()) {
    win->write_line("-- INSERT --", 0, 0);
  } else if (model->get_cmd_so_far().size() > 0) {
    win->write_line(model->get_cmd_so_far(), 0, 0);
  } else {
    win->write_line("\"" + model->get_file_name() + "\"", 0, 0);
  }

  string y_pos = model->getLines()->at(model->getY()).size() == 0
                     ? "0-1"
                     : to_string(model->getX() + 1);
  string pos = to_string(model->getY() + 1) + "," + y_pos;
  win->write_line(pos, 0.75 * win->getWidth(), 0);

  win->refresh_w();
}
