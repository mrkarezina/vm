#include "view_text.h"

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
  int off = ceil_div(model->get_lines()->at(ln).size(), win->getWidth());
  // If line is empty, still count as 1 y-offset
  return (off == 0 ? 1 : off);
}

int ViewText::get_view_y(TextModel *model) {
  int y_offset = 0;
  for (int i = 0; i < model->get_y(); i++) {
    y_offset += get_y_offset(i, model);
  }
  // Line wrap for current line
  y_offset += model->get_x() / win->getWidth();
  return y_offset;
}

int ViewText::get_view_x(TextModel *model) {
  return model->get_x() % win->getWidth();
}

void ViewText::set_scroll_focus_y(TextModel *model) {
  int max_diff = 0.75 * win->getHeight();
  int max_close = 0.25 * win->getHeight();
  int y = get_view_y(model);

  // Scroll down if Y greater than what fits in window
  if (y - scroll_focus_y > max_diff) scroll_focus_y = y - max_diff;
  // Scroll up
  if (y - scroll_focus_y < max_close && scroll_focus_y > 0) {
    scroll_focus_y = y - max_close;
  }
}

void ViewText::draw(TextModel *model) {
  win->erase_w();
  const shared_ptr<vector<string>> lines = model->get_lines();

  set_scroll_focus_y(model);

  int y_offset = 0;
  for (size_t i = 0; i < lines->size(); i++) {
    // Check if current line within scroll location
    if (scroll_focus_y <= y_offset &&
        y_offset < scroll_focus_y + win->getHeight()) {
      win->write_line(lines->at(i), 0, y_offset - scroll_focus_y);
    }
    y_offset += get_y_offset(i, model);
  }

  for (size_t i = y_offset - scroll_focus_y; i < win->getHeight(); i++) {
    draw_tilda(0, i);
  }

  win->move_w(get_view_x(model), get_view_y(model) - scroll_focus_y);
  win->refresh_w();
}
