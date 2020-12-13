#include "parser.h"

#include "cmd_basic.h"
#include "cmd_fileio.h"
#include "cmd_motion.h"
#include "cmd_multi.h"
#include "cmd_page_nav.h"
#include "cmd_search.h"
#include "util.h"

using namespace std;

Parser::Parser(TextModel *model) : model{model} { initilize_cmd_lists(); }

void Parser::initilize_cmd_lists() {
  motion_cmd_list = {'h', 'j', 'k', 'l', 'w', 'b', '0', '^', '$'};
  insert_cmd_list = {'i', 'I', 'a', 'A', 'o', 'O'};
}

// Converts ^[character] to the character
// https://en.wikipedia.org/wiki/Control_character#How_control_characters_map_to_keyboards
int Parser::convert_ctrl(int c) { return c + 64; }

std::unique_ptr<CmdBase> Parser::parse_wait_until_enter(int c) {
  string cur_cmd = model->get_cmd_so_far();

  // Search commands
  // cur_cmd.substr(1, cur_cmd.size() - 2, get's _the_text_ from
  // /_the_text_[enter acsii]
  if ((cur_cmd[0] == '/' || cur_cmd[0] == '?') && cur_cmd.size() > 2 && c == 10)
    return make_unique<CmdSearchStart>(cur_cmd[0],
                                       cur_cmd.substr(1, cur_cmd.size() - 2));

  // TODO: if :q and :w comes first they will always match first
  // Makes code depending on order and brittle
  if (cur_cmd.substr(0, 3) == ":q!" && c == 10) return make_unique<CmdQuit>();
  if (cur_cmd.substr(0, 3) == ":wq" && c == 10)
    return make_unique<CmdSaveExit>();
  if (cur_cmd.substr(0, 2) == ":q" && c == 10) return make_unique<CmdQuit>();
  if (cur_cmd.substr(0, 2) == ":w" && c == 10)
    return make_unique<CmdSaveLines>();

  // Line selection
  if (cur_cmd[0] == ':' && cur_cmd.size() > 1 && c == 10)
    // Remove the : and enter key at the back
    return make_unique<CmdLineSelection>(cur_cmd.substr(1, cur_cmd.size() - 2));

  return nullptr;
}

std::unique_ptr<CmdBase> Parser::parse_command_mode(int c) {
  string cur_cmd = model->get_cmd_so_far();

  if (cur_cmd[0] == ':' || cur_cmd[0] == '/' || cur_cmd[0] == '?')
    stall_until_enter = true;

  if (stall_until_enter) return parse_wait_until_enter(c);

  if (vec_contains<char>(motion_cmd_list, c)) {
    return make_unique<CmdMove>(c);
  }
  if (vec_contains<char>(insert_cmd_list, c)) {
    return make_unique<CmdInsert>(c);
  }

  // Clear any motion
  if (cur_cmd[0] == 'c' && vec_contains<char>(motion_cmd_list, c))
    return make_unique<CmdcC>(c);

  // Delete any motion
  if (cur_cmd[0] == 'd' && vec_contains<char>(motion_cmd_list, c))
    return make_unique<CmddD>(c);

  // Copy any motion
  if (cur_cmd[0] == 'y' && vec_contains<char>(motion_cmd_list, c))
    return make_unique<CmdyY>(c);

  if (cur_cmd == "cc") return make_unique<CmdcC>('c');
  if (cur_cmd == "yy") return make_unique<CmdyY>('y');
  if (cur_cmd == "dd") return make_unique<CmddD>('d');
  if (cur_cmd == "x") return make_unique<CmddD>('x');

  // Search
  if (cur_cmd.size() > 1 && cur_cmd[0] == 'f') {
    return make_unique<Cmdf>(c);
  }
  if (cur_cmd == ";") {
    return make_unique<Cmdf>('\0');
  }

  // Substitute commands
  if (cur_cmd == "s" || cur_cmd == "S") return make_unique<CmdsS>(c);

  // Paste commands
  if (cur_cmd == "p" || cur_cmd == "P") return make_unique<CmdPaste>(c);

  if (cur_cmd == "n" || cur_cmd == "N") return make_unique<CmdSearchNav>(c);

  if (convert_ctrl(c) == 'G') return make_unique<CmdCtrlG>();
  if (convert_ctrl(c) == 'F') return make_unique<CmdCtrlF>();
  if (convert_ctrl(c) == 'B') return make_unique<CmdCtrlB>();
  if (convert_ctrl(c) == 'U') return make_unique<CmdCtrlU>();
  if (convert_ctrl(c) == 'D') return make_unique<CmdCtrlD>();

  return nullptr;
}

std::unique_ptr<CmdBase> Parser::parse_any_mode(int c) {
  // Commands for both command / write mode
  if (c == 10) return make_unique<CmdEnter>();
  if (c == 127 || c == KEY_BACKSPACE) return make_unique<CmdBackspace>();
  if (c == 27) return make_unique<CmdEsc>();

  if (c == KEY_UP) return make_unique<CmdMove>('k');
  if (c == KEY_DOWN) return make_unique<CmdMove>('j');

  // Any character written in write mode
  if (model->is_write_mode()) {
    return make_unique<CmdWrite>(c);
  }

  return nullptr;
}

void Parser::reset() { stall_until_enter = false; }
