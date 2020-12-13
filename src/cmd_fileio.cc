#include "cmd_fileio.h"

using namespace std;

void CmdSaveLines::exec(TextModel *model) { model->save_lines(); }

void CmdQuit::exec(TextModel *model) { model->set_render_loop_off(); }

CmdSaveExit::CmdSaveExit() {
  add_command(std::move(make_shared<CmdSaveLines>()));
  add_command(std::move(make_shared<CmdQuit>()));
}
