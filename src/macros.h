#ifndef MACROS_H
#define MACROS_H

#include <map>
#include <memory>
#include <vector>

class CmdBase;

class Macros {
  std::map<char, std::vector<std::shared_ptr<CmdBase>>> recorded_commands;

  bool recording_macro;
  bool to_record;
  char current_letter;

 public:
  Macros();
  void record_command(std::shared_ptr<CmdBase> cmd);
  // Generates multi command composed of recorded commands
  std::shared_ptr<CmdBase> generate_playback_command(char macro);
  // Clear previous recording
  void clear(char macro);

  // Set letter to associate recording to
  void set_current_letter(char macro);
  char get_current_letter();

  bool is_recording_macro();
  void set_recording_macro(bool show);

  void set_to_record(bool record);
};

#endif
