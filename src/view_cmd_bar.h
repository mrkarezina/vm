#ifndef VIEW_CMD_BAR_H
#define VIEW_CMD_BAR_H

#include <memory>

#include "text_model.h"
#include "view_base.h"
#include "window.h"

class ViewCmdBar : public ViewBase {
 private:
  std::unique_ptr<Window> win;

  std::string scroll_percentage(TextModel *model);
  std::string percentage_status(TextModel *model);
  void draw_cursor_posn(TextModel *model);

 public:
  ViewCmdBar();
  void draw(TextModel *model) override;
};

#endif
