#include "cmd_multi.h"

using namespace std;

void CmdMultiCommand::add_command(std::unique_ptr<CmdBase> cmd) {
  commands.emplace_back(std::move(cmd));
}

void CmdMultiCommand::exec(TextModel *model) {
  for (auto &command : commands) {
    model->apply(std::move(command));
  }
}

