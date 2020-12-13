#include "view_cmd_bar.h"

using namespace std;

ViewCmdBar::ViewCmdBar() {
  int height, width;
  getmaxyx(stdscr, height, width);
  win = make_unique<Window>(width, 1, 0, height - 1);
}

string ViewCmdBar::scroll_percentage(TextModel *model) {
  float perc = (float)model->get_y() / (float)model->get_lines()->size() * 100;
  return to_string((int)perc) + "%%";
}

string ViewCmdBar::percentage_status(TextModel *model) {
  if (model->get_y() == model->get_lines()->size() - 1) {
    return "Bot";
  } else if (model->get_y() == 0) {
    return "Top";
  } else {
    return scroll_percentage(model);
  }
}

void ViewCmdBar::draw_cursor_posn(TextModel *model) {
  string y_pos = model->get_lines()->at(model->get_y()).size() == 0
                     ? "0-1"
                     : to_string(model->get_x() + 1);
  string pos = to_string(model->get_y() + 1) + "," + y_pos;
  string perc = percentage_status(model);
  win->write_line(pos, 0.75 * win->getWidth(), 0);
  win->write_line(perc, win->getWidth() - 4, 0);
}

void ViewCmdBar::draw(TextModel *model) {
  win->erase_w();
  if (model->is_write_mode()) {
    win->write_line("-- INSERT --", 0, 0);
  } else if (model->get_cmd_so_far().size() > 0) {
    win->write_line(model->get_cmd_so_far(), 0, 0);
  } else if (model->is_show_file_stats()) {
    win->write_line("\"" + model->get_file_name() + "\" " +
                        to_string((int)model->get_lines()->size()) + " lines " +
                        " --" + scroll_percentage(model) + "-- ",
                    0, 0);
    // Don't show after next command
    model->set_show_file_stats(false);
  } else {
    win->write_line("\"" + model->get_file_name() + "\"", 0, 0);
  }

  draw_cursor_posn(model);

  win->refresh_w();
}
