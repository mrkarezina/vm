#ifndef CMD_BASIC_H
#define CMD_BASIC_H

#include "cmd-base.h"

class CmdWrite : public CmdBase {
 char toWrite;
 public:
  CmdWrite(char c);
  void exec(TextModel *model);
};

class CmdDel : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdEsc : public CmdBase {
  public:
    void exec(TextModel *model);
};


#endif
