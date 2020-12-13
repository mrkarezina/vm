#ifndef PARSER_H
#define PARSER_H

#include <ncurses.h>

#include <memory>
#include <string>
#include <vector>

#include "cmd_base.h"

class TextModel;

class Parser {
  std::vector<char> motion_cmd_list;
  std::vector<char> insert_cmd_list;

  TextModel *model;

  void initilize_cmd_lists();
  int convert_ctrl(int c);

  /**
   * For commands requiring full user input before enter.
   * Signals to ignore all other command mode commands until enter is pressed
   */
  bool stall_until_enter = false;
  bool is_multiplier = false;

  int multiplier = 0;

 public:
  Parser(TextModel *model);
  /**
   * Parses commands requiring all input until enter key.
   * : ? /
   */
  std::unique_ptr<CmdBase> parse_wait_until_enter(int c);
  /**
   * Attempts to parse command mode commands based on
   * current c and text model state.
   *
   * If no command matched returns nullptr
   */
  std::unique_ptr<CmdBase> parse_command_mode(int c);
  std::unique_ptr<CmdBase> parse_insert_mode(int c);
  std::unique_ptr<CmdBase> parse_any_mode(int c);
  std::unique_ptr<CmdBase> parse_multiplier(int c);

  /**
   * Resets temporary state if command matched
   */
  void reset();
};

#endif
