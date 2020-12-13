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

std::shared_ptr<CmdBase> Parser::parse_wait_until_enter(int c) {
  string cur_cmd = model->get_cmd_so_far();

  // Search commands
  // cur_cmd.substr(1, cur_cmd.size() - 2, get's _the_text_ from
  // /_the_text_[enter acsii]
  if ((cur_cmd[0] == '/' || cur_cmd[0] == '?') && cur_cmd.size() > 2 && c == 10)
    return make_shared<CmdSearchStart>(cur_cmd[0],
                                       cur_cmd.substr(1, cur_cmd.size() - 2));

  // TODO: if :q and :w comes first they will always match first
  // Makes code depending on order and brittle
  if (cur_cmd.substr(0, 3) == ":q!" && c == 10) return make_shared<CmdQuit>();
  if (cur_cmd.substr(0, 3) == ":wq" && c == 10)
    return make_shared<CmdSaveExit>();
  if (cur_cmd.substr(0, 2) == ":q" && c == 10) return make_shared<CmdQuit>();
  if (cur_cmd.substr(0, 2) == ":w" && c == 10)
    return make_shared<CmdSaveLines>();

  // Line selection
  if (cur_cmd[0] == ':' && cur_cmd.size() > 1 && c == 10)
    // Remove the : and enter key at the back
    return make_shared<CmdLineSelection>(cur_cmd.substr(1, cur_cmd.size() - 2));

  return nullptr;
}

std::shared_ptr<CmdBase> Parser::parse_command_mode(int c) {
  string cur_cmd = model->get_cmd_so_far();

  if (cur_cmd[0] == ':' || cur_cmd[0] == '/' || cur_cmd[0] == '?')
    stall_until_enter = true;

  if (stall_until_enter) return parse_wait_until_enter(c);

  if (vec_contains<char>(motion_cmd_list, c)) {
    return make_shared<CmdMove>(c);
  }
  if (vec_contains<char>(insert_cmd_list, c)) {
    return make_shared<CmdInsert>(c);
  }

  // Clear any motion
  if (cur_cmd[0] == 'c' && vec_contains<char>(motion_cmd_list, c))
    return make_shared<CmdcC>(c);

  // Delete any motion
  if (cur_cmd[0] == 'd' && vec_contains<char>(motion_cmd_list, c))
    return make_shared<CmddD>(c);

  // Copy any motion
  if (cur_cmd[0] == 'y' && vec_contains<char>(motion_cmd_list, c))
    return make_shared<CmdyY>(c);

  if (cur_cmd == "cc") return make_shared<CmdcC>('c');
  if (cur_cmd == "yy") return make_shared<CmdyY>('y');
  if (cur_cmd == "dd") return make_shared<CmddD>('d');
  if (cur_cmd == "x") return make_shared<CmddD>('x');

  // Search
  if (cur_cmd.size() > 1 && cur_cmd[0] == 'f') {
    return make_shared<Cmdf>(c);
  }
  if (cur_cmd == ";") {
    return make_shared<Cmdf>('\0');
  }

  // Substitute commands
  if (cur_cmd == "s" || cur_cmd == "S") return make_shared<CmdsS>(c);

  // Paste commands
  if (cur_cmd == "p" || cur_cmd == "P") return make_shared<CmdPaste>(c);

  if (cur_cmd == "n" || cur_cmd == "N") return make_shared<CmdSearchNav>(c);

  if (convert_ctrl(c) == 'G') return make_shared<CmdCtrlG>();
  if (convert_ctrl(c) == 'F') return make_shared<CmdCtrlF>();
  if (convert_ctrl(c) == 'B') return make_shared<CmdCtrlB>();
  if (convert_ctrl(c) == 'U') return make_shared<CmdCtrlU>();
  if (convert_ctrl(c) == 'D') return make_shared<CmdCtrlD>();

  return nullptr;
}

std::shared_ptr<CmdBase> Parser::parse_any_mode(int c) {
  // Commands for both command / write mode
  if (c == 10) return make_shared<CmdEnter>();
  if (c == 127 || c == KEY_BACKSPACE) return make_shared<CmdBackspace>();
  if (c == 27) return make_shared<CmdEsc>();

  if (c == KEY_UP) return make_shared<CmdMove>('k');
  if (c == KEY_DOWN) return make_shared<CmdMove>('j');

  // Any character written in write mode
  if (model->is_write_mode()) {
    return make_shared<CmdWrite>(c);
  }

  return nullptr;
}

std::shared_ptr<CmdBase> Parser::parse_multiplier(int c) {
  string cur_cmd = model->get_cmd_so_far();

  // Possible multiplier at begining of command
  if (!model->is_write_mode() && cur_cmd.size() == 0 && isdigit(c)) {
    is_multiplier = true;
    return make_shared<CmdStall>();
  }
  if (is_multiplier && isdigit(c)) {
    return make_shared<CmdStall>();
  }
  // Once commands starts, parse the multipler digit
  // and remove digit from current command for other parsers to work
  if (is_multiplier && !isdigit(c)) {
    multiplier = stoi(cur_cmd.substr(0, cur_cmd.size() - 1));
    // Set command to first non digit character
    cur_cmd = cur_cmd.substr(cur_cmd.size() - 1);
  }

  // If not multiplier or the end of a multiplier stop stalling
  is_multiplier = false;
  return nullptr;
}

void Parser::reset() {
  stall_until_enter = false;
  is_multiplier = false;
  multiplier = 0;
}
