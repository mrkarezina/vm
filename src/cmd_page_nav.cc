#include "cmd_page_nav.h"

using namespace std;

void CmdCtrlG::exec(TextModel *model) { model->set_show_file_stats(true); }

int calculate_page_size() {
  int height, width;
  getmaxyx(stdscr, height, width);
  return height;
}

void CmdCtrlU::exec(TextModel *model) {
  // Decrease y posn by half a page
  int new_y = max(model->get_y() - (int)(0.5 * calculate_page_size()), 0);
  model->set_y(new_y);
}

void CmdCtrlD::exec(TextModel *model) {
  // Increase y posn by half a page
  int new_y = min(model->get_y() + (int)(0.5 * calculate_page_size()),
                  (int)model->get_lines()->size() - 1);
  model->set_y(new_y);
}

void CmdCtrlB::exec(TextModel *model) {
  // Decrease y posn by half a page
  int new_y = max(model->get_y() - calculate_page_size(), 0);
  model->set_y(new_y);
}

void CmdCtrlF::exec(TextModel *model) {
  // Increase y posn by half a page
  int new_y = min(model->get_y() + calculate_page_size(),
                  (int)model->get_lines()->size() - 1);
  model->set_y(new_y);
}
