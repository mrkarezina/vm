#ifndef APP_H
#define APP_H

#include <iostream>
#include <memory>
#include <string>
#include <ncurses.h>

#include "text_model.h"
#include "view-base.h"
#include "view-text.h"

class App {
  // TODO: make this a unqiue pointer, requires copy constructor
  std::shared_ptr<TextModel> text_model;

 public:
  App();
  ~App(){};
  void run();
};

#endif
