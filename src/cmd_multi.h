#ifndef CMD_MULTI_H
#define CMD_MULTI_H

#include <memory>
#include <vector>

#include "cmd_base.h"

class TextModel;

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

#endif
