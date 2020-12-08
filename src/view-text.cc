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

void ViewText::draw(TextModel *model) {
  win->erase_w();
  const shared_ptr<vector<string>> lines = model->getLines();
  for (size_t i = 0; i < lines->size(); i++) {
    string l = lines->at(i);
    for (size_t j = 0; j < l.size(); j++) {
      win->write_char(l[j], j, i);
      // cout << l[j];
    }
    // cout << endl;
  }

  for(size_t i = lines->size(); i < win->getHeight(); i++) {
    draw_tilda(0, i);
  }

  win->move_w(model->getX(), model->getY());
  win->refresh_w();
}
