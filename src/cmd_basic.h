#ifndef CMD_BASIC_H
#define CMD_BASIC_H

#include <ncurses.h>

#include <stdexcept>

#include "cmd_base.h"
#include "posn.h"
#include "text_model.h"
#include "cmd_util.h"

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

class CmdEnter : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdStall : public CmdBase {
 public:
  void exec(TextModel *model);
};

/**
 * Combine two lines together.
 */
class CmdJ : public CmdBase {
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
 * Paste commands
 * p, P
 */
class CmdPaste : public CmdBase {
  char paste_type;

 public:
  CmdPaste(char c);
  void exec(TextModel *model);
};

class CmdUndo : public CmdBase {
 public:
  void exec(TextModel *model);
};

#endif
