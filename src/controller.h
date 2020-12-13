#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ncurses.h>

#include <map>
#include <memory>
#include <string>

#include "cmd_basic.h"
#include "cmd_fileio.h"
#include "cmd_motion.h"
#include "cmd_search.h"

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
