#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <memory>
#include <string>

#include "renderer.h"
#include "view-text.h"

class TextModel {
  std::string text;
  std::unique_ptr<Renderer> renderer;

 public:
  TextModel(std::string &filename);
  ~TextModel(){};
  void run();
};

#endif
