#ifndef CMD_PAGE_NAV_H
#define CMD_PAGE_NAV_H

#include "cmd_base.h"
#include "text_model.h"

class CmdCtrlG : public CmdBase {
 public:
  void exec(TextModel *model);
};

#endif
