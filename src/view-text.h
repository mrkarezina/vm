#ifndef VIEW_TEXT_H
#define VIEW_TEXT_H

#include <iostream>
#include <memory>

#include "text_model.h"
#include "view-base.h"
#include "window.h"

class ViewText : public ViewBase {
  std::unique_ptr<Window> win;
  void draw_tilda(int x, int y);
  int get_view_x(TextModel *model);
  int get_view_y(TextModel *model);
 public:
  ViewText();
  void draw(TextModel *model) override;
};

#endif
