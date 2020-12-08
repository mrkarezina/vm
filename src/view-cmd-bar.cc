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
  } else {
    win->write_line("\"" + model->get_file_name() + "\"", 0, 0);
  }
  win->refresh_w();
}
