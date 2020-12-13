#ifndef CMD_BASIC_H
#define CMD_BASIC_H

#include <ncurses.h>

#include <stdexcept>

#include "cmd-base.h"
#include "posn.h"
#include "text_model.h"

class CmdWrite : public CmdBase {
  char toWrite;

 public:
  CmdWrite(char c);
  void exec(TextModel *model);
};

class CmdBackspace : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdEsc : public CmdBase {
 public:
  void exec(TextModel *model);
};

/**
 * Handles i, I, a, A, o, O
 */
class CmdInsert : public CmdBase {
  char insert_type;

 public:
  CmdInsert(char c);
  void exec(TextModel *model);
};

class CmdEnter : public CmdBase {
 public:
  void exec(TextModel *model);
};

/**
 * Supports h, j, k, l
 */
class CmdMove : public CmdBase {
  char move;

 public:
  CmdMove(char c);
  void exec(TextModel *model);
};

class CmdStall : public CmdBase {
 public:
  void exec(TextModel *model);
};

/**
 * Used to create composite commands.
 * For example:
 * 3j
 * - Array of 3, j commands
 * :wq
 * - Array of w and q commands
 */
class CmdMultiCommand : public CmdBase {
  std::vector<std::unique_ptr<CmdBase>> commands;

 public:
  void add_command(std::unique_ptr<CmdBase>);
  void exec(TextModel *model);
};

class CmdSaveLines : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdQuit : public CmdBase {
 public:
  void exec(TextModel *model);
};

/**
 * Composite command of write and quit
 */
class CmdSaveExit : public CmdMultiCommand {
 public:
  CmdSaveExit();
};

/**
 * Line selection commands:
 * :0, :$, :line-number
 */
class CmdLineSelection : public CmdBase {
  std::string selection;

 public:
  CmdLineSelection(std::string selection);
  void exec(TextModel *model);
};

/**
 * Supports f[some char] command.
 *
 * Supports ',' find next command.
 * Setting to_find to empty char '\0' will restore
 * previously searched for char and find next occurance.
 */
class Cmdf : public CmdBase {
  char to_find;

 public:
  Cmdf(char c);
  void exec(TextModel *model);
};

/**
 * Substitute commands
 * s, S
 */
class CmdsS : public CmdBase {
  char sub_type;

 public:
  CmdsS(char c);
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
 * d[any motion], dd, x
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

/**
 * Paste commands
 * p, P
 */
class CmdPaste : public CmdBase {
  char paste_type;

 public:
  CmdPaste(char c);
  void exec(TextModel *model);
};

#endif
