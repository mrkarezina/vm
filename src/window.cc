#include "window.h"

using namespace std;

Window::Window(float w_perc, float h_perc, int x, int y) {
  getmaxyx(stdscr, height, width);
  win = newwin(height * h_perc, width * w_perc, y, x);
}

Window::Window(int width, int height, int x, int y)
    : width{width}, height{height} {
  win = newwin(height, width, y, x);
}

Window::~Window() { delwin(win); }

int Window::getHeight() { return height; }
int Window::getWidth() { return width; }

void Window::refresh_w() { wrefresh(win); }

void Window::erase_w() { werase(win); }

void Window::write_char(char c, int x, int y) {
  mvwaddch(win, y, x, c);
}

void Window::write_line(std::string s, int x, int y) {
  mvwprintw(win, y, x, s.c_str());
}

void Window::write_char(char c) { waddch(win, c); }

void Window::move_w(int x, int y) { wmove(win, y, x); }
