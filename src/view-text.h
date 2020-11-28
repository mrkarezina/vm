#ifndef VIEW_TEXT_H
#define VIEW_TEXT_H

#include <iostream>

#include "view-base.h"

class ViewText : public ViewBase {
 public:
  ViewText(){};
  void draw() override;
};

#endif
