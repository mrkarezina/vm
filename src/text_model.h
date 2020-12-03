#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "view-base.h"
#include "controller.h"
#include "cmd-base.h"

class ViewBase;
class CmdBase;
class Controller;

// TODO: need seperate structure for data location
// Ie: wrapped line, does not mean writing to 'lines' second line
struct Cursor {
  int x;
  int y;
};

class TextModel {
  std::shared_ptr<std::vector<std::string>> lines;
  std::vector<std::unique_ptr<ViewBase>> views;
  Cursor cur_posn{0, 0};
  std::unique_ptr<Controller> controller;

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void addView(std::unique_ptr<ViewBase> view);
  void render();

  void apply(std::unique_ptr<CmdBase> cmd);

  const std::shared_ptr<std::vector<std::string>> getLines();
  void writeChar(char c, int x, int y);
  void addChar(char c, int x, int y);

  void setX(int x);
  void setY(int y);
  int getX();
  int getY();
};

#endif
