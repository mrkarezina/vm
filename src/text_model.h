#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "view-base.h"

class ViewBase;

class TextModel {
  std::shared_ptr<std::vector<std::string>> lines;

  std::vector<std::unique_ptr<ViewBase>> views;

 public:
  TextModel(std::string &filename);
  ~TextModel();
  void run();

  void addView(std::unique_ptr<ViewBase> view);
  void render();

  const std::shared_ptr<std::vector<std::string>> getLines();
};

#endif
