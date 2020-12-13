#ifndef CMD_PAGE_NAV_H
#define CMD_PAGE_NAV_H

#include <ncurses.h>

#include "cmd_base.h"
#include "text_model.h"

class CmdCtrlG : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdCtrlU : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdCtrlD : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdCtrlB : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdCtrlF : public CmdBase {
 public:
  void exec(TextModel *model);
};

#endif
