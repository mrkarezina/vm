#include "window.h"

using namespace std;

Window::Window(float w_perc, float h_perc, int x, int y) {
  getmaxyx(stdscr, height, width);
  win = newwin(height * h_perc, width * w_perc, y, x);
}

Window::~Window() { delwin(win); }

void Window::refresh() { wrefresh(win); }

void Window::writeChar(char c, int x, int y) {
  move(x, y);
  waddch(win, c);
}

void Window::writeChar(char c) { waddch(win, c); }

void Window::move(int x, int y) { wmove(win, y, x); }
