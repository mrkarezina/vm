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

App::App() {
  // TODO: parse command line args
  initialize_curses();
  string m = "tests/test.txt";
  text_model = make_shared<TextModel>(m);
  text_model->addView(unique_ptr<ViewCmdBar>(new ViewCmdBar()));
  text_model->addView(unique_ptr<ViewText>(new ViewText()));
}

void App::run() { 
  text_model->run();
 }
