#include "cmd_macro.h"

CmdStartMacro::CmdStartMacro(char macro) : macro{macro} {}

void CmdStartMacro::exec(TextModel *model) {
  model->macros->clear(macro);
  model->macros->set_current_letter(macro);
  model->macros->set_recording_macro(true);
}

void CmdEndMacro::exec(TextModel *model) {
  model->macros->set_recording_macro(false);
}

CmdPlayBackMacro::CmdPlayBackMacro(char macro) : macro{macro} {}

void CmdPlayBackMacro::exec(TextModel *model) {
  model->apply(model->macros->generate_playback_command(macro));
}
