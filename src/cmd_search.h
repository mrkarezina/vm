#ifndef CMD_SEARCH_H
#define CMD_SEARCH_H

#include <stdexcept>
#include <string>

#include "cmd_base.h"
#include "posn.h"
#include "text_model.h"

/**
 * Base class for search commands
 * ? / n N
 */
class CmdSearchBase : public CmdBase {
 protected:
  // Returns posn{-1, -1} if string not found
  Posn find_next_occurance(TextModel *model, std::string query, Posn start_pos);
  Posn find_prev_occurance(TextModel *model, std::string query, Posn start_pos);
};

/**
 * Search
 * ? /
 */
class CmdSearchStart : public CmdSearchBase {
  char search_type;
  std::string query;

 public:
  CmdSearchStart(char search_type, std::string query);
  void exec(TextModel *model);
};

/**
 * Search navigation
 * n N
 */
class CmdSearchNav : public CmdSearchBase {
  char nav_type;

 public:
  CmdSearchNav(char c);
  void exec(TextModel *model);
};

#endif
