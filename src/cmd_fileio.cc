#include "cmd_fileio.h"

using namespace std;

void CmdSaveLines::exec(TextModel *model) {
  model->save_lines();
  model->history->change_since_write = 1;
}

void CmdQuit::exec(TextModel *model) { model->set_render_loop_off(); }

void CmdQuitWarning::exec(TextModel *model) {
  if (model->history->change_since_write <= 1) {
    model->set_render_loop_off();
  } else {
    model->set_display_save_warning(true);
  }
}

CmdSaveExit::CmdSaveExit() {
  add_command(make_shared<CmdSaveLines>());
  add_command(make_shared<CmdQuit>());
}
