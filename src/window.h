#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

#include <string>

class Window {
 private:
  WINDOW *win;
  int height;
  int width;

 public:
  Window();
  Window(float w_perc, float h_perc, int x, int y);
  Window(int width, int height, int x, int y);
  ~Window();

  int getHeight();
  int getWidth();

  void refresh_w();
  void erase_w();
  void write_char(char c);
  void write_line(std::string s, int x, int y);
  void write_char(char c, int x, int y);
  void move_w(int x, int y);
};

#endif
