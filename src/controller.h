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

  /**
   * Create a vector of n instances of the same command shared_ptr. Since
   * commands are stateless can apply the same instance of the command on text
   * model multiple times.
   */
  std::shared_ptr<CmdBase> multiply_command(std::shared_ptr<CmdBase> cmd);

 public:
  Controller(TextModel *model);
  std::shared_ptr<CmdBase> parse_input();
};

#endif
