#include "clipboard.h"

void Clipboard::set_paste_on_newline(bool b) { paste_on_newline = b; }

void Clipboard::add_line(std::string s) { lines.push_back(s); }

const std::vector<std::string> &Clipboard::get_lines() { return lines; }

const bool Clipboard::is_paste_on_newline() { return paste_on_newline; }

void Clipboard::clear() {
  paste_on_newline = false;
  lines.clear();
}
