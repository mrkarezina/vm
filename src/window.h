#ifndef WINDOW_H
#define WINDOW_H

#include "ncurses.h"

class Window {
 private:
  WINDOW *win;
  int height;
  int width;

 public:
  Window();
  Window(float h_perc, float w_perc, int x, int y);
  ~Window();

  void refresh_w();
  void erase_w();
  void writeChar(char c);
  void writeChar(char c, int x, int y);
  void move_w(int x, int y);

};

#endif
