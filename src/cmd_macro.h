#ifndef CMD_MACRO_H
#define CMD_MACRO_H

#include "cmd_base.h"
#include "text_model.h"

/**
 * Start recording macro associated with character.
 */
class CmdStartMacro : public CmdBase {
  char macro;

 public:
  CmdStartMacro(char macro);
  void exec(TextModel *model);
};

class CmdEndMacro : public CmdBase {
 public:
  void exec(TextModel *model);
};

class CmdPlayBackMacro : public CmdBase {
  char macro;

 public:
  CmdPlayBackMacro(char macro);
  void exec(TextModel *model);
};

/**
 * Plays back the previous command, if there is a previous command.
 */
class CmdPlaybackPrev : public CmdBase {
 public:
  void exec(TextModel *model);
};

#endif
