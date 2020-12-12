#include "app.h"

using namespace std;

void initialize_curses() {
  // ncurses initialization
  initscr();
  refresh();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

App::App(std::string file) {
  initialize_curses();
  text_model = make_shared<TextModel>(file);
  text_model->add_view(unique_ptr<ViewCmdBar>(new ViewCmdBar()));
  text_model->add_view(unique_ptr<ViewText>(new ViewText()));
}

void App::run() { text_model->run(); }
