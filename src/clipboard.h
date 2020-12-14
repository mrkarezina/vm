#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
#include <vector>

class Clipboard {
  std::vector<std::string> lines;
  bool paste_on_newline;

 public:
  void set_paste_on_newline(bool b);
  const bool is_paste_on_newline();
  void add_line(std::string s);
  const std::vector<std::string> &get_lines();
  void clear();
};

#endif
