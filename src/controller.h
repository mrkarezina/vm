#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ncurses.h>

#include <map>
#include <memory>
#include <string>

#include "cmd_basic.h"
#include "parser.h"

class CmdBase;
class TextModel;

class Controller {
 private:
  TextModel *model;
  std::unique_ptr<Parser> parser;

 public:
  Controller(TextModel *model);
  std::unique_ptr<CmdBase> parse_input();
};

#endif
