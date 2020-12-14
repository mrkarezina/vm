#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

#include "posn.h"

class CmdBase;

class TextModel;

class History {
  std::vector<std::vector<std::string>> text_states;
  std::vector<Posn> cursor_states;

  std::shared_ptr<CmdBase> prev;

  bool replace_prev_command = false;

 public:
  History();
  void record_state(TextModel *model);
  std::vector<std::string> pop_last_text_state();
  Posn pop_last_cursor_state();
  std::shared_ptr<CmdBase> get_prev_cmd();
  void set_prev_cmd(std::shared_ptr<CmdBase> cmd);

  void record_prev_command(std::shared_ptr<CmdBase> cmd);
  /**
   * Indicates not to replace the prev command in history.
   * Used by motion commands.
   */ 
  void set_replace_prev_command(bool replace);

  int state_size();
  int change_since_write;
};

#endif
