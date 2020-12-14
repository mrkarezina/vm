#ifndef CMD_MOTION_H
#define CMD_MOTION_H

#include <ncurses.h>

#include "cmd_base.h"
#include "text_model.h"

class CmdMoveBase : public CmdBase {
 protected:
  Posn move_h(TextModel *model);
  Posn move_l(TextModel *model);
  Posn move_k(TextModel *model);
  Posn move_j(TextModel *model);
  Posn start_next_word(TextModel *model);
  Posn start_prev_word(TextModel *model);
};

/**
 * Supports h, j, k, l
 */
class CmdMove : public CmdMoveBase {
  char move_type;

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
