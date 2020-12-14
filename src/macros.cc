#include "macros.h"

#include "cmd_multi.h"

using namespace std;

Macros::Macros() { recording_macro = false; }

void Macros::record_command(std::shared_ptr<CmdBase> cmd) {
  recorded_commands[current_letter].push_back(cmd);
}

void Macros::set_current_letter(char macro) { current_letter = macro; }

shared_ptr<CmdBase> Macros::generate_playback_command(char macro) {
  shared_ptr<CmdMultiCommand> multi = make_shared<CmdMultiCommand>();
  for (auto cmd : recorded_commands[macro]) {
    multi->add_command(cmd);
  }
  return multi;
}

void Macros::clear(char macro) { recorded_commands[macro].clear(); }

bool Macros::is_recording_macro() { return recording_macro; }

void Macros::set_recording_macro(bool record) { recording_macro = record; }
