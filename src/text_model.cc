#include "text_model.h"

using namespace std;

class text_range_access {};

TextModel::TextModel(string &filename) : filename{filename} {
  controller = make_unique<Controller>(this);
  write_mode = false;
  cursor_posn = Posn(0, 0);
  clipboard = make_unique<Clipboard>();
  history = make_unique<History>();
  macros = make_unique<Macros>();

  // lines = make_shared<vector<string>>();
  // lines->push_back("Test 1 Test 1 Test 1 Test 1 Test 1Test 1 Test 1 Test 1
  // Test 1 Test 1 Test 1 Test 1 Test 1 Test 1 Test 1 Test 1");
  // lines->push_back("Test 2");
  if (filename.size() > 0) {
    lines = load_lines(filename);
  } else {
    lines = make_shared<vector<string>>();
    lines->push_back("");
  }
}

TextModel::~TextModel() {}

void TextModel::run() {
  while (render_loop_on) {
    render();
    apply(controller->parse_input());
  }
}

void TextModel::toggle_mode() { write_mode = !write_mode; }

bool TextModel::is_write_mode() { return write_mode; }

void TextModel::set_render_loop_off() { render_loop_on = false; }

bool TextModel::is_show_file_stats() { return show_file_stats; }

void TextModel::set_show_file_stats(bool show) { show_file_stats = show; }

bool TextModel::is_display_save_warning() { return display_save_warning; }

void TextModel::set_display_save_warning(bool show) {
  display_save_warning = show;
}

string TextModel::get_file_name() { return filename; }

void TextModel::save_lines() { write_lines(filename, lines); }

string TextModel::get_cmd_so_far() { return cmd_so_far; }

void TextModel::set_cmd_so_far(string cmd) { cmd_so_far = cmd; }

const shared_ptr<vector<string>> TextModel::get_lines() { return lines; }

void TextModel::set_lines(std::vector<std::string> ln) { *lines = ln; }

string TextModel::get_line_at(int y) {
  return lines->at(y == -1 ? get_y() : y);
}

void TextModel::set_line_at(string s, int y) {
  lines->at(y == -1 ? get_y() : y) = s;
}

void TextModel::write_char(char c, int x, int y) { lines->at(y)[x] = c; }

void TextModel::add_char(char c, int x, int y) {
  while (y >= lines->size()) {
    lines->push_back("");
  }
  if (x > lines->at(y).size()) {
    throw text_range_access();
  }
  // Add character before cursor (so right at index)
  lines->at(y).insert(x, string{c});
}

// Adds a newline splitting text line in two
void TextModel::new_line(int x, int y) {
  string left = lines->at(y).substr(0, x);
  string right = lines->at(y).substr(x);
  lines->at(y) = left;
  lines->insert(lines->begin() + y + 1, right);
}

void TextModel::delete_line_concat(int y) {
  string right = lines->at(y);
  lines->at(y - 1) = lines->at(y - 1) + right;
  lines->erase(lines->begin() + y);
}

void TextModel::delete_line(int y) { lines->erase(lines->begin() + y); }

void TextModel::clear_line(int y) { lines->at(y) = ""; }

void TextModel::delete_char(int x, int y) { lines->at(y).erase(x - 1, 1); }

void TextModel::set_x(int x) { cursor_posn.x = x; }

void TextModel::set_y(int y) { cursor_posn.y = y; }

int TextModel::get_x() { return cursor_posn.x; }

int TextModel::get_y() { return cursor_posn.y; }

Posn TextModel::get_posn() { return cursor_posn; }

void TextModel::set_posn(Posn &pos) { cursor_posn = pos; }

void TextModel::add_view(unique_ptr<ViewBase> view) {
  views.push_back(move(view));
}

void TextModel::apply(shared_ptr<CmdBase> cmd) {
  history->record_state(this);
  cmd->exec(this);
  macros->record_command(cmd);
  history->record_prev_command(cmd);
}

void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
