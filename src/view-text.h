#ifndef VIEW_TEXT_H
#define VIEW_TEXT_H

#include <iostream>

#include "view-base.h"
#include "text_model.h"
#include "window.h"

class ViewText : public ViewBase {
 public:
  ViewText() {};
  void draw(TextModel *model) override;
};

#endif
