#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

#include "posn.h"

class TextModel;

class History {
  std::vector<std::vector<std::string>> text_states;
  std::vector<Posn> cursor_states;

 public:
  History();
  void record_state(TextModel *model);
  std::vector<std::string> pop_last_text_state();
  Posn pop_last_cursor_state();
  int state_size();
  int change_since_write;
};

#endif
