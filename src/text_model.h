#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "view-base.h"

class ViewBase;

struct Cursor {
  int x;
  int y;
};

class TextModel {
  std::shared_ptr<std::vector<std::string>> lines;
  std::vector<std::unique_ptr<ViewBase>> views;
  Cursor cur_posn{0, 0};

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void addView(std::unique_ptr<ViewBase> view);
  void render();

  const std::shared_ptr<std::vector<std::string>> getLines();
  void setX(int x);
  void setY(int y);
  int getX();
  int getY();
};

#endif
