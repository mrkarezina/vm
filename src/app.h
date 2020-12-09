#ifndef APP_H
#define APP_H

#include <ncurses.h>

#include <iostream>
#include <memory>
#include <string>

#include "text_model.h"
#include "view-cmd-bar.h"
#include "view-text.h"

class App {
  // TODO: make this a unqiue pointer, requires copy constructor
  std::shared_ptr<TextModel> text_model;

 public:
  App(std::string file);
  ~App(){};
  void run();
};

#endif
