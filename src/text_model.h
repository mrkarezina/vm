#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "clipboard.h"
#include "cmd_base.h"
#include "controller.h"
#include "file_util.h"
#include "history.h"
#include "macros.h"
#include "posn.h"
#include "view_base.h"

class Controller;

class TextModel {
  std::shared_ptr<std::vector<std::string>> lines;
  std::vector<std::unique_ptr<ViewBase>> views;
  Posn cursor_posn;
  std::unique_ptr<Controller> controller;

  std::string filename;
  bool render_loop_on = true;
  bool write_mode;
  std::string cmd_so_far;

  bool show_file_stats = false;
  bool display_save_warning = false;

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void add_view(std::unique_ptr<ViewBase> view);
  void render();

  void apply(std::shared_ptr<CmdBase> cmd);

  void set_render_loop_off();
  void toggle_mode();
  bool is_write_mode();

  bool is_show_file_stats();
  void set_show_file_stats(bool show);
  bool is_display_save_warning();
  void set_display_save_warning(bool show);

  std::string get_file_name();
  void save_lines();

  std::string get_cmd_so_far();
  void set_cmd_so_far(std::string cmd);
  void save_state_data(std::string key, std::string data);
  void save_state_data(std::string key, int data);

  const std::shared_ptr<std::vector<std::string>> get_lines();
  void set_lines(std::vector<std::string> ln);
  // Defaults to current cursor y position
  std::string get_line_at(int y = -1);
  void set_line_at(std::string s, int y = -1);
  void write_char(char c, int x, int y);
  void add_char(char c, int x, int y);
  void new_line(int x, int y);
  void delete_char(int x, int y);
  /**
   * Deletes line at 'y' and concacenates its contents to the line above.
   * Equivilent to pressing backspace at begining of line.
   */
  void delete_line_concat(int y);
  void delete_line(int y);
  void clear_line(int y);

  void set_x(int x);
  void set_y(int y);
  int get_x();
  int get_y();
  Posn get_posn();
  void set_posn(Posn &pos);

  std::unique_ptr<Clipboard> clipboard;
  std::unique_ptr<History> history;
  std::unique_ptr<Macros> macros;

  std::map<std::string, std::string> state_str;
  std::map<std::string, int> state_int;
};

#endif
