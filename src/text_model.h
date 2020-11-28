#ifndef TEXT_MODEL_H
#define TEXT_MODEL_H

#include <iostream>
#include <string>

class TextModel {
  std::string text;

 public:
  TextModel(std::string &filename);
  ~TextModel(){};
};

#endif
