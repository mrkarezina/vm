#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <memory>
#include <string>

#include "cmd-base.h"
#include "cmd-basic.h"
#include "cmd-motion.h"
#include "cmd-search.h"
#include "ncurses.h"
#include "text_model.h"

class CmdBase;
class TextModel;

class Controller {
  // Needs to figure out command
  // Provide input to command

  // Keyboard maps input to command -> initlizes command and stuff

  // Keep insert / write state in model
  // Each key try to map "cur_cmd" string to command
  // map for insert mode
  // map for cmd mode
  // store info to provide to cmd, ie: char

 private:
  std::map<std::string, std::string> write_cmd_map;
  std::map<std::string, std::string> insert_cmd_map;

  void initilize_write_cmd_map();
  void initilize_insert_cmd_map();

  TextModel *model;

 public:
  Controller(TextModel *model);
  std::unique_ptr<CmdBase> parse_input();
};

#endif
