#include "view-cmd-bar.h"

using namespace std;

ViewCmdBar::ViewCmdBar() {
  int height, width;
  getmaxyx(stdscr, height, width);
  win = make_unique<Window>(width, 1, 0, height - 1);
}

void ViewCmdBar::draw(TextModel *model) {
  win->erase_w();
  win->write_line("Command bar ...", 0, 0);
  win->refresh_w();
}
