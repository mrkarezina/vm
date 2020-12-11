#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cmd-base.h"
#include "controller.h"
#include "file_util.h"
#include "view-base.h"

class ViewBase;
class CmdBase;
class Controller;

// TODO: view should caculate wrap (+ offest for cursor on wrapped line).
// Cursor points to write posn in data.
struct Cursor {
  int x;
  int y;
};

class TextModel {
  std::shared_ptr<std::vector<std::string>> lines;
  std::vector<std::unique_ptr<ViewBase>> views;
  Cursor cur_posn{0, 0};
  std::unique_ptr<Controller> controller;
  std::string filename;
  bool render_loop_on = true;
  bool write_mode;
  std::string cmd_so_far;

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void addView(std::unique_ptr<ViewBase> view);
  void render();

  void apply(std::unique_ptr<CmdBase> cmd);

  void toggle_mode();
  void set_render_loop_off();
  bool is_write_mode();
  std::string get_file_name();
  void save_lines();

  std::string get_cmd_so_far();
  void set_cmd_so_far(std::string cmd);
  void save_state_data(std::string key, std::string data);
  void save_state_data(std::string key, int data);

  const std::shared_ptr<std::vector<std::string>> getLines();
  void writeChar(char c, int x, int y);
  void addChar(char c, int x, int y);
  void new_line(int x, int y);
  void delete_char(int x, int y);
  void delete_line(int x, int y, bool concat);

  void setX(int x);
  void setY(int y);
  int getX();
  int getY();

  // Arbitrary state data for commands
  // TODO: getter setters
  std::map<std::string, std::string> state_str;
  std::map<std::string, int> state_int;
};

#endif
