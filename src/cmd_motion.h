#ifndef CMD_MOTION_H
#define CMD_MOTION_H

#include <ncurses.h>

#include "cmd_base.h"
#include "text_model.h"

class CmdMoveBase : public CmdBase {
 protected:
  // Left
  Posn move_h(TextModel *model);
  // Right
  Posn move_l(TextModel *model);
  // Up
  Posn move_k(TextModel *model);
  // Down
  Posn move_j(TextModel *model);

  // Front of line
  Posn move_0(TextModel *model);
  // Location of first word in line
  Posn move_caret(TextModel *model);
  // End of line
  Posn move_dollar(TextModel *model);

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
class CmdcC : public CmdMoveBase {
  char change_type;

  /**
   * Same as delete_core except will clear instead of delete
   * the end.y line.
   */
  void clear_core(TextModel *model, Posn end);

 public:
  CmdcC(char c);
  void exec(TextModel *model);
};

/**
 * Deletion commands
 * d[any motion], dd, x, X
 */
class CmddD : public CmdMoveBase {
  char del_type;

  /**
   * Deletes from the current position to the end position
   */
  void delete_core(TextModel *model, Posn end);

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
