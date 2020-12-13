#ifndef VIEW_CMD_BAR_H
#define VIEW_CMD_BAR_H

#include <memory>

#include "view_base.h"
#include "text_model.h"
#include "window.h"

class ViewCmdBar : public ViewBase {
 private:
  std::unique_ptr<Window> win;

 public:
  ViewCmdBar();
  void draw(TextModel *model) override;
};

#endif
