#include "cmd_util.h"

#include "text_model.h"

void ensure_y_in_bounds(TextModel *model) {
  // If last line deleted, need to move y back
  if (model->get_y() >= model->get_lines()->size() &&
      model->get_lines()->size() > 0) {
    model->set_y(model->get_lines()->size() - 1);
  }
}
