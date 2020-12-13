#ifndef CMD_MOTION_H
#define CMD_MOTION_H

#include <ncurses.h>

#include "cmd_base.h"
#include "text_model.h"

/**
 * Supports h, j, k, l
 */
class CmdMove : public CmdBase {
  char move_type;

  Posn start_next_word(TextModel *model);
  Posn start_prev_word(TextModel *model);

 public:
  CmdMove(char c);
  void exec(TextModel *model);
};

/**
 * Supports e, b, 0, ^, $
 */
class CmdJump : public CmdBase {
  char jump_type;

 public:
  CmdJump(char c);
  void exec(TextModel *model);
};

/**
 * Change replace commands
 * c[any motion], cc
 */
class CmdcC : public CmdBase {
  char change_type;

 public:
  CmdcC(char c);
  void exec(TextModel *model);
};

/**
 * Deletion commands
 * d[any motion], dd, x, X
 */
class CmddD : public CmdBase {
  char del_type;

 public:
  CmddD(char c);
  void exec(TextModel *model);
};

/**
 * Yank commands
 * y[any motion], yy, x
 */
class CmdyY : public CmdBase {
  char yank_type;

 public:
  CmdyY(char c);
  void exec(TextModel *model);
};

#endif
