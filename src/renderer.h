#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>

#include "view-base.h"
#include "view-text.h"

class Renderer {
  std::vector<std::unique_ptr<ViewBase>> views;

 public:
  Renderer();
  void addView(std::unique_ptr<ViewBase> view);
  void render();
};

#endif
