#ifndef CMD_FILEIO_H
#define CMD_FILEIO_H

#include "cmd_base.h"
#include "text_model.h"
#include "cmd_multi.h"

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

#endif
