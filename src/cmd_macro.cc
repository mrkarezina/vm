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
  model->macros->set_to_record(false);
}

void CmdPlaybackPrev::exec(TextModel *model) {
  if (model->history->get_prev_cmd() != nullptr) {
    std::shared_ptr<CmdBase> prev = model->history->get_prev_cmd();
    model->apply(prev);
  }
  // Prevent recording undo command resulting in infinite loop
  model->history->set_replace_prev_command(false);
  // Don't record the playback command, rather the commands themselves
  model->macros->set_to_record(false);
}
