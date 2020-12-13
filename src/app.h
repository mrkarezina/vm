#ifndef APP_H
#define APP_H

#include <ncurses.h>

#include <iostream>
#include <memory>
#include <string>

#include "text_model.h"
#include "view_cmd_bar.h"
#include "view_text.h"

class App {
  // TODO: make this a unqiue pointer, requires copy constructor
  std::shared_ptr<TextModel> text_model;

 public:
  App(std::string file);
  ~App(){};
  void run();
};

#endif
