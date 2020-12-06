#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cmd-base.h"
#include "controller.h"
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
  bool write_mode;

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void addView(std::unique_ptr<ViewBase> view);
  void render();

  void apply(std::unique_ptr<CmdBase> cmd);

  void toggle_mode();

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
};

#endif
